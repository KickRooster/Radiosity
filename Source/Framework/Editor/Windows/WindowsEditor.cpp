#include "WindowsEditor.h"

#include <DirectXMath.h>
#include <SDL/SDL_filesystem.h>
#include "../../../Helper/TextureOperator.h"
#include "../3rdParty/LinearLeastSquaresStitch/Sitich.h"
#include "../../../Helper/Helper.h"
#include "LinearColor.h"

#include STRING_INCLUDE_PATH
#include VECTOR_INCLUDE_PATH

using namespace ctd;
using namespace std;
using namespace rbp;

namespace Core
{
	void WindowsEditor::createBuiltinResources()
	{
		m_arealLightMaterial = std::make_shared<Material>();

		m_arealLightMaterial->glVertexShader = m_assetManager->glVertexShaderMap["light"];
		m_arealLightMaterial->glVertexShader.lock()->Attach(m_arealLightMaterial.get());
		m_arealLightMaterial->glFragmentShader = m_assetManager->glFragmentShaderMap["light"];
		m_arealLightMaterial->glFragmentShader.lock()->Attach(m_arealLightMaterial.get());

		m_arealLightMaterial->albedoTexture = m_assetManager->textureMap["default"];
		m_arealLightMaterial->albedoTexture.lock()->Attach(m_arealLightMaterial.get());
		
		m_arealLightMaterial->lightmapTexture = m_assetManager->lightmapMap["DummyLightmap"];
		m_arealLightMaterial->lightmapTexture.lock()->Attach(m_arealLightMaterial.get());
		m_arealLightMaterial->IsOccluder = True;
		//////////////////////////////////////////////////////////////////////////

		m_DrawGBufferMaterial = std::make_shared<Material>();
		m_DrawGBufferMaterial->glVertexShader = m_assetManager->glVertexShaderMap["drawGBuffer"];
		m_DrawGBufferMaterial->glVertexShader.lock()->Attach(m_DrawGBufferMaterial.get());
		m_DrawGBufferMaterial->glFragmentShader = m_assetManager->glFragmentShaderMap["drawGBuffer"];
		m_DrawGBufferMaterial->glFragmentShader.lock()->Attach(m_DrawGBufferMaterial.get());

		//////////////////////////////////////////////////////////////////////////

		m_DrawIDMaterial = std::make_shared<Material>();
		m_DrawIDMaterial->glVertexShader = m_assetManager->glVertexShaderMap["drawID"];
		m_DrawIDMaterial->glVertexShader.lock()->Attach(m_DrawIDMaterial.get());
		m_DrawIDMaterial->glFragmentShader = m_assetManager->glFragmentShaderMap["drawID"];
		m_DrawIDMaterial->glFragmentShader.lock()->Attach(m_DrawIDMaterial.get());

		//////////////////////////////////////////////////////////////////////////

		m_ComputeFormFactorMaterial = std::make_shared<Material>();
		m_ComputeFormFactorMaterial->glVertexShader = m_assetManager->glVertexShaderMap["computeFormFactor"];
		m_ComputeFormFactorMaterial->glVertexShader.lock()->Attach(m_ComputeFormFactorMaterial.get());
		m_ComputeFormFactorMaterial->glFragmentShader = m_assetManager->glFragmentShaderMap["computeFormFactor"];
		m_ComputeFormFactorMaterial->glFragmentShader.lock()->Attach(m_ComputeFormFactorMaterial.get());
		
		//////////////////////////////////////////////////////////////////////////
		m_DefaultLightMesh = std::make_shared<StaticMesh>();

		int32 XCount = 1;
		int32 YCount = 1;
		float XCountF = static_cast<float>(XCount);
		float YCountF = static_cast<float>(XCount);

		m_DefaultLightMesh->vertexCount = XCount * YCount * 6;
		m_DefaultLightMesh->pPositions = new Vector4[m_DefaultLightMesh->vertexCount];
		m_DefaultLightMesh->pNormals = new Vector3[m_DefaultLightMesh->vertexCount];
		m_DefaultLightMesh->pUV0s = new Vector2[m_DefaultLightMesh->vertexCount];
		m_DefaultLightMesh->pUV1s = new Vector2[m_DefaultLightMesh->vertexCount];
		
		m_DefaultLightMesh->indexCount = XCount * YCount * 6;
		m_DefaultLightMesh->pIndices = new uint32[m_DefaultLightMesh->indexCount];
		
		const Vector3 PhysicalSize = Vector3(1, 1, 0);
		Vector4 LightScale = Vector4(PhysicalSize.x / static_cast<float>(XCount), PhysicalSize.y / static_cast<float>(YCount), 0, 1.0);
		int32 VertexCursor = 0;
		
		for (int32 i = 0; i < YCount; ++i)
		{ 
			for (int32 j = 0; j < XCount; ++j)
			{
				Vector4 P0 = Vector4(-static_cast<float>(XCount) / 2.0f +  static_cast<float>(j), -static_cast<float>(YCount) / 2.0f + static_cast<float>(i), 0, 1.0f);
				P0 *= LightScale;
				Vector4 P1 = Vector4(-static_cast<float>(XCount) / 2.0f +  static_cast<float>(j) + 1.0f, -static_cast<float>(YCount) / 2.0f + static_cast<float>(i), 0, 1.0f);
				P1 *= LightScale;
				Vector4 P2 = Vector4(-static_cast<float>(XCount) / 2.0f +  static_cast<float>(j), -static_cast<float>(YCount) / 2.0f + static_cast<float>(i) + 1.0f, 0, 1.0f);
				P2 *= LightScale;
				
				Vector4 P3 = Vector4(-static_cast<float>(XCount) / 2.0f +  static_cast<float>(j) + 1.0f, -static_cast<float>(YCount) / 2.0f + static_cast<float>(i), 0, 1.0f);
				P3 *= LightScale;
				Vector4 P4 = Vector4(-static_cast<float>(XCount) / 2.0f +  static_cast<float>(j) + 1.0f, -static_cast<float>(YCount) / 2.0f + static_cast<float>(i) + 1.0f, 0, 1.0f);
				P4 *= LightScale;
				Vector4 P5 = Vector4(-static_cast<float>(XCount) / 2.0f +  static_cast<float>(j), -static_cast<float>(YCount) / 2.0f + static_cast<float>(i) + 1.0f, 0, 1.0);
				P5 *= LightScale;
				
				m_DefaultLightMesh->pPositions[VertexCursor] = P0;
				m_DefaultLightMesh->pNormals[VertexCursor] = Vector3(0, 0, 1.0);
				Vector2 UV0 = Vector2(static_cast<float>(j) / XCountF, static_cast<float>(i) / YCountF);
				m_DefaultLightMesh->pUV0s[VertexCursor] = UV0;
				m_DefaultLightMesh->pUV1s[VertexCursor] = UV0;
				m_DefaultLightMesh->pIndices[VertexCursor] = VertexCursor;
				++VertexCursor;
				
				m_DefaultLightMesh->pPositions[VertexCursor] = P1;
				m_DefaultLightMesh->pNormals[VertexCursor] = Vector3(0, 0, 1.0);
				Vector2 UV1 = Vector2((static_cast<float>(j) + 1.0f) / XCountF, static_cast<float>(i) / YCountF);
				m_DefaultLightMesh->pUV0s[VertexCursor] = UV1;
				m_DefaultLightMesh->pUV1s[VertexCursor] = UV1;
				m_DefaultLightMesh->pIndices[VertexCursor] = VertexCursor;
				++VertexCursor;

				m_DefaultLightMesh->pPositions[VertexCursor] = P2;
				m_DefaultLightMesh->pNormals[VertexCursor] = Vector3(0, 0, 1.0);
				Vector2 UV2 = Vector2(static_cast<float>(j) / XCountF, (static_cast<float>(i) + 1.0f) / YCountF);
				m_DefaultLightMesh->pUV0s[VertexCursor] = UV2;
				m_DefaultLightMesh->pUV1s[VertexCursor] = UV2;
				m_DefaultLightMesh->pIndices[VertexCursor] = VertexCursor;
				++VertexCursor;
				
				m_DefaultLightMesh->pPositions[VertexCursor] = P3;
				m_DefaultLightMesh->pNormals[VertexCursor] = Vector3(0, 0, 1.0);
				Vector2 UV3 = Vector2((static_cast<float>(j) + 1.0f) / XCountF, static_cast<float>(i) / YCountF);
				m_DefaultLightMesh->pUV0s[VertexCursor] = UV3;
				m_DefaultLightMesh->pUV1s[VertexCursor] = UV3;
				m_DefaultLightMesh->pIndices[VertexCursor] = VertexCursor;
				++VertexCursor;

				m_DefaultLightMesh->pPositions[VertexCursor] = P4;
				m_DefaultLightMesh->pNormals[VertexCursor] = Vector3(0, 0, 1.0);
				Vector2 UV4 = Vector2((static_cast<float>(j) + 1.0f) / XCountF, (static_cast<float>(i) + 1.0f) / YCountF);
				m_DefaultLightMesh->pUV0s[VertexCursor] = UV4;
				m_DefaultLightMesh->pUV1s[VertexCursor] = UV4;
				m_DefaultLightMesh->pIndices[VertexCursor] = VertexCursor;
				++VertexCursor;

				m_DefaultLightMesh->pPositions[VertexCursor] = P5;
				m_DefaultLightMesh->pNormals[VertexCursor] = Vector3(0, 0, 1.0);
				Vector2 UV5 = Vector2(static_cast<float>(j) / XCountF, (static_cast<float>(i) + 1.0f) / YCountF);
				m_DefaultLightMesh->pUV0s[VertexCursor] = UV5;
				m_DefaultLightMesh->pUV1s[VertexCursor] = UV5;
				m_DefaultLightMesh->pIndices[VertexCursor] = VertexCursor;
				++VertexCursor;
			}
		}
	}

	std::shared_ptr<StaticMesh> WindowsEditor::CreateAreaLightMesh(int32 XLength, int32 YLength, LightPrecision Precision)
	{
		float XLengthF = static_cast<float>(XLength);
		float YLengthF = static_cast<float>(YLength);
		
		float TriangleCountPerSize = GetLightTriangleCountPerSide(Precision);

		float TriangleCountX = XLengthF * TriangleCountPerSize;
		float TriangleCountY = YLengthF * TriangleCountPerSize;

		float TriangleCountXUpper = ceil(TriangleCountX);
		float TriangleCountYUpper = ceil(TriangleCountY);

		//	For check calculation result.
		float CalculatedXLength = TriangleCountXUpper / TriangleCountPerSize;
		float CalculatedYLength = TriangleCountYUpper / TriangleCountPerSize;

		float XCountF = TriangleCountXUpper;
		float YCountF = TriangleCountYUpper;

		int32 XCount = static_cast<int32>(XCountF);
		int32 YCount = static_cast<int32>(YCountF);
		
		std::shared_ptr<StaticMesh> Mesh = std::make_shared<StaticMesh>();
		
		Mesh->vertexCount = XCount * YCount * 6;
		Mesh->pPositions = new Vector4[Mesh->vertexCount];
		Mesh->pNormals = new Vector3[Mesh->vertexCount];
		Mesh->pUV0s = new Vector2[Mesh->vertexCount];
		Mesh->pUV1s = new Vector2[Mesh->vertexCount];
		
		Mesh->indexCount = XCount * YCount * 6;
		Mesh->pIndices = new uint32[Mesh->indexCount];
		
		Vector3 PhysicalSize = Vector3(XLength, YLength, 0);
		Vector4 LightScale = Vector4(PhysicalSize.x / static_cast<float>(XCount), PhysicalSize.y / static_cast<float>(YCount), 0, 1.0);
		int32 VertexCursor = 0;
		
		for (int32 i = 0; i < YCount; ++i)
		{ 
			for (int32 j = 0; j < XCount; ++j)
			{
				Vector4 P0 = Vector4(-static_cast<float>(XCount) / 2.0f +  static_cast<float>(j), -static_cast<float>(YCount) / 2.0f + static_cast<float>(i), 0, 1.0f);
				P0 *= LightScale;
				Vector4 P1 = Vector4(-static_cast<float>(XCount) / 2.0f +  static_cast<float>(j) + 1.0f, -static_cast<float>(YCount) / 2.0f + static_cast<float>(i), 0, 1.0f);
				P1 *= LightScale;
				Vector4 P2 = Vector4(-static_cast<float>(XCount) / 2.0f +  static_cast<float>(j), -static_cast<float>(YCount) / 2.0f + static_cast<float>(i) + 1.0f, 0, 1.0f);
				P2 *= LightScale;
				
				Vector4 P3 = Vector4(-static_cast<float>(XCount) / 2.0f +  static_cast<float>(j) + 1.0f, -static_cast<float>(YCount) / 2.0f + static_cast<float>(i), 0, 1.0f);
				P3 *= LightScale;
				Vector4 P4 = Vector4(-static_cast<float>(XCount) / 2.0f +  static_cast<float>(j) + 1.0f, -static_cast<float>(YCount) / 2.0f + static_cast<float>(i) + 1.0f, 0, 1.0f);
				P4 *= LightScale;
				Vector4 P5 = Vector4(-static_cast<float>(XCount) / 2.0f +  static_cast<float>(j), -static_cast<float>(YCount) / 2.0f + static_cast<float>(i) + 1.0f, 0, 1.0);
				P5 *= LightScale;
				
				Mesh->pPositions[VertexCursor] = P0;
				Mesh->pNormals[VertexCursor] = Vector3(0, 0, 1.0);
				Vector2 UV0 = Vector2(static_cast<float>(j) / XCountF, static_cast<float>(i) / YCountF);
				Mesh->pUV0s[VertexCursor] = UV0;
				Mesh->pUV1s[VertexCursor] = UV0;
				Mesh->pIndices[VertexCursor] = VertexCursor;
				++VertexCursor;
				
				Mesh->pPositions[VertexCursor] = P1;
				Mesh->pNormals[VertexCursor] = Vector3(0, 0, 1.0);
				Vector2 UV1 = Vector2((static_cast<float>(j) + 1.0f) / XCountF, static_cast<float>(i) / YCountF);
				Mesh->pUV0s[VertexCursor] = UV1;
				Mesh->pUV1s[VertexCursor] = UV1;
				Mesh->pIndices[VertexCursor] = VertexCursor;
				++VertexCursor;

				Mesh->pPositions[VertexCursor] = P2;
				Mesh->pNormals[VertexCursor] = Vector3(0, 0, 1.0);
				Vector2 UV2 = Vector2(static_cast<float>(j) / XCountF, (static_cast<float>(i) + 1.0f) / YCountF);
				Mesh->pUV0s[VertexCursor] = UV2;
				Mesh->pUV1s[VertexCursor] = UV2;
				Mesh->pIndices[VertexCursor] = VertexCursor;
				++VertexCursor;
				
				Mesh->pPositions[VertexCursor] = P3;
				Mesh->pNormals[VertexCursor] = Vector3(0, 0, 1.0);
				Vector2 UV3 = Vector2((static_cast<float>(j) + 1.0f) / XCountF, static_cast<float>(i) / YCountF);
				Mesh->pUV0s[VertexCursor] = UV3;
				Mesh->pUV1s[VertexCursor] = UV3;
				Mesh->pIndices[VertexCursor] = VertexCursor;
				++VertexCursor;

				Mesh->pPositions[VertexCursor] = P4;
				Mesh->pNormals[VertexCursor] = Vector3(0, 0, 1.0);
				Vector2 UV4 = Vector2((static_cast<float>(j) + 1.0f) / XCountF, (static_cast<float>(i) + 1.0f) / YCountF);
				Mesh->pUV0s[VertexCursor] = UV4;
				Mesh->pUV1s[VertexCursor] = UV4;
				Mesh->pIndices[VertexCursor] = VertexCursor;
				++VertexCursor;

				Mesh->pPositions[VertexCursor] = P5;
				Mesh->pNormals[VertexCursor] = Vector3(0, 0, 1.0);
				Vector2 UV5 = Vector2(static_cast<float>(j) / XCountF, (static_cast<float>(i) + 1.0f) / YCountF);
				Mesh->pUV0s[VertexCursor] = UV5;
				Mesh->pUV1s[VertexCursor] = UV5;
				Mesh->pIndices[VertexCursor] = VertexCursor;
				++VertexCursor;
			}
		}
		
		return Mesh;
	}
	
	void WindowsEditor::panelSceneObjects()
	{
		ImGui::SetNextWindowPos(ImVec2(400, 600));
		ImGui::SetNextWindowSize(ImVec2(200, 200));

		ImGui::Begin("Scene Objects");
		{
			for (ctd::vector<std::shared_ptr<Object>> ::iterator iter = m_scene->objects.begin();
				iter != m_scene->objects.end();
				++iter)
			{
				bool selected = iter->get() == m_pSelectedObject;

				bool selectedLastFrame = selected;

				ImGui::Selectable(iter->get()->name.c_str(), &selected);

				if (selectedLastFrame && !selected)
					m_pSelectedObject = Null;
				else if (selected)
					m_pSelectedObject = iter->get();
			}
		}
		ImGui::End();
	}

	void WindowsEditor::popupNamingTick(const ANSICHAR *pName, const ANSICHAR *pExtension, InputState & inputState, Bool & pressedOK, ctd::string & fileName)
	{
		if (ImGui::BeginPopupModal(pName))
		{
			pressedOK = False;
			inputState.inlvaid = True;

			ImGui::Text("Input the file name: ");

			const int32 maxNameLength = 63;
			static ANSICHAR buffer[maxNameLength + 1] = "";
			ImGui::InputText(pExtension, buffer, maxNameLength + 1);// , ImGuiInputTextFlags_CharsNoBlank);

			if (ImGui::Button("OK"))
			{
				ImGui::CloseCurrentPopup();
				pressedOK = True;
				
				fileName = buffer;
			}

			ImGui::SameLine();

			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}
	}

	void WindowsEditor::popupFileSelectingTick(FileType fileType, const ANSICHAR *pName, bool * pfileSelections, int32 & selectedIndex, int32 & selectedIndexLastFrame, InputState & inputState, Bool & pressedOK, ctd::string & fileNameWithExt)
	{
		if (ImGui::BeginPopupModal(pName))
		{
			inputState.inlvaid = True;
			int32 index = 0;
			static string selectedFileName;

			switch (fileType)
			{
			case FileType_StaticMesh:
				for (map<string, shared_ptr<StaticMesh>>::iterator iter = m_assetManager->staticMeshMap.begin();
					iter != m_assetManager->staticMeshMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), pfileSelections + index, ImGuiSelectableFlags_DontClosePopups))
					{
						selectedIndex = index;
						selectedFileName = iter->first;
						break;
					}

					++index;
				}
				break;
			case FileType_Material:
				for (map<string, shared_ptr<Material>>::iterator iter = m_assetManager->materialMap.begin();
					iter != m_assetManager->materialMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), pfileSelections + index, ImGuiSelectableFlags_DontClosePopups))
					{
						selectedIndex = index;
						selectedFileName = iter->first;
						break;
					}

					++index;
				}
				break;
			case FileType_Texture:
				for (map<string, shared_ptr<Texture>>::iterator iter = m_assetManager->textureMap.begin();
					iter != m_assetManager->textureMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), pfileSelections + index, ImGuiSelectableFlags_DontClosePopups))
					{
						selectedIndex = index;
						selectedFileName = iter->first;
						break;
					}
					
					++index;
				}
				break;
			case FileType_Lightmap:
				for (map<string, shared_ptr<Texture>>::iterator iter = m_assetManager->lightmapMap.begin();
					iter != m_assetManager->lightmapMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), pfileSelections + index, ImGuiSelectableFlags_DontClosePopups))
					{
						selectedIndex = index;
						selectedFileName = iter->first;
						break;
					}
					
					++index;
				}
				break;
			case FileType_GLSL_Vertex:
				for (map<string, shared_ptr<GLSL>>::iterator iter = m_assetManager->glVertexShaderMap.begin();
					iter != m_assetManager->glVertexShaderMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), pfileSelections + index, ImGuiSelectableFlags_DontClosePopups))
					{
						selectedIndex = index;
						selectedFileName = iter->first;
						break;
					}

					++index;
				}
				break;
			case FileType_GLSL_Fragment:
				for (map<string, shared_ptr<GLSL>>::iterator iter = m_assetManager->glFragmentShaderMap.begin();
					iter != m_assetManager->glFragmentShaderMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), pfileSelections + index, ImGuiSelectableFlags_DontClosePopups))
					{
						selectedIndex = index;
						selectedFileName = iter->first;
						break;
					}

					++index;
				}
				break;
			default:
				break;
			}

			if (selectedIndex != selectedIndexLastFrame)
			{
				if (selectedIndexLastFrame != InvalidIndex)
					pfileSelections[selectedIndexLastFrame] = false;
			}
			else
				pfileSelections[selectedIndex] = true;

			selectedIndexLastFrame = selectedIndex;

			if (ImGui::Button("OK"))
			{
				pressedOK = true;
				fileNameWithExt = selectedFileName;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}
	}

	void WindowsEditor::panelAssets(InputState & inputState)
	{
		ImGui::SetNextWindowPos(ImVec2(600, 600));
		ImGui::SetNextWindowSize(ImVec2(1200, 400));

		ImGui::Begin("Assets");
		{
			inputState.inlvaid = False;
			//ImGui::TextColored(ImVec4(0, 0.6f, 1.0f, 1.0f), "Create: ");

			Bool pressedOK = False;
			string fileName;

			// if (ImGui::Button("Create Scene"))
			// 	ImGui::OpenPopup("Scene");
			//
			// popupNamingTick("Scene", m_assetManager->sceneExt, inputState, pressedOK, fileName);
			//
			// if (pressedOK)
			// {
			// 	m_assetManager->CreateScene(fileName.c_str());
			// 	pressedOK = False;
			// }
			//
			// ImGui::SameLine();

			// if (ImGui::Button("Create Prefab"))
			// 	ImGui::OpenPopup("Prefab");
			//
			// popupNamingTick("Prefab", m_assetManager->prefabExt, inputState, pressedOK, fileName);
			//
			// if (pressedOK)
			// {
			// 	m_assetManager->CreatePrefab(fileName.c_str());
			// 	pressedOK = False;
			// }
			//
			// ImGui::SameLine();

			// if (ImGui::Button("Create Material"))
			// 	ImGui::OpenPopup("Material");
			//
			// popupNamingTick("Material", m_assetManager->materialExt, inputState, pressedOK, fileName);
			//
			// if (pressedOK)
			// {
			// 	m_assetManager->CreateMaterial(fileName.c_str());
			// 	pressedOK = False;
			// }
			//
			// ImGui::SameLine();

			if (ImGui::Button("Save All"))
				m_assetManager->SaveAll();

			//////////////////////////////////////////////////////////////////////////
			ImGui::Columns(3);

			static AssetType selectedCategory = AssetType_None;
			static AssetType selectedCategoryLastFrame = AssetType_None;

			//	���IMGUI,��Ҫ��bool������Bool.��ĿͬCategory��Ŀһ��.
			static bool categorySelections[10] = { False, False, False, False, False, False, False, False , False};

			ctd::string categoryName = "";

			const int32 maxFileSize = 200;
			static bool fileSelections[maxFileSize] = { false };
			static bool fileSelectionsInInspector[maxFileSize] = { false };
			static int32 selectedFileIndex = InvalidIndex;
			static int32 selectedFileLastFrameIndex = InvalidIndex;
			static int32 selectedFileIndexInInspector = InvalidIndex;
			static int32 selectedFileLastFrameIndexInInspector = InvalidIndex;

			//////////////////////////////////////////////////////////////////////////
			//	Category

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild("Category", ImVec2(0, 300), true, 0);

			//if (ImGui::Selectable("Scene", &categorySelections[AssetType_Scene]))
			//{
			//	selectedCategory = AssetType_Scene;
			//	categoryName = "Scene";
			//}

			//if (ImGui::Selectable("Prefab", &categorySelections[AssetType_Prefab]))
			//{
			//	selectedCategory = AssetType_Prefab;
			//	categoryName = "Prefab";
			//}

			if (ImGui::Selectable("Static Mesh", &categorySelections[AssetType_StaticMesh]))
			{
				selectedCategory = AssetType_StaticMesh;
				categoryName = "Static Mesh";
			}

			if (ImGui::Selectable("Material", &categorySelections[AssetType_Material]))
			{
				selectedCategory = AssetType_Material;
				categoryName = "Material";
			}

			if (ImGui::Selectable("Texture", &categorySelections[AssetType_Texture]))
			{
				selectedCategory = AssetType_Texture;
				categoryName = "Texture";
			}

			if (ImGui::Selectable("Lightmap", &categorySelections[AssetType_Lightmap]))
			{
				selectedCategory = AssetType_Lightmap;
				categoryName = "Lightmap";
			}

			//if (ImGui::Selectable("GLSL", &categorySelections[AssetType_GLShader]))
			//{
			//	selectedCategory = AssetType_GLShader;
			//	categoryName = "GLSL";
			//}
			
			if (selectedCategory != selectedCategoryLastFrame)
			{
				if (selectedCategoryLastFrame != AssetType_None)
					categorySelections[selectedCategoryLastFrame] = false;

				memset(fileSelections, false, sizeof(fileSelections[0]) * maxFileSize);
				selectedFileIndex = InvalidIndex;
				selectedFileLastFrameIndex = InvalidIndex;
			}
			else
			{
				categorySelections[selectedCategory] = true;
			}

			selectedCategoryLastFrame = selectedCategory;

			ImGui::EndChild();
			ImGui::PopStyleVar();

			ImGui::NextColumn();

			//	File

			int32 index = 0;
			static weak_ptr<Scene> selectedScene;
			static weak_ptr<Prefab> selectedPrefab;
			static weak_ptr<StaticMesh> selectedStaticMesh;
			static weak_ptr<Material> selectedMaterial;
			static weak_ptr<Texture> selectedTexture;

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild(categoryName.c_str(), ImVec2(0, 300), true, 0);

			switch (selectedCategory)
			{
			case Core::AssetType_Scene:
				for (map<string, shared_ptr<Scene>>::iterator iter = m_assetManager->sceneMap.begin();
					iter != m_assetManager->sceneMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), &fileSelections[index]))
					{
						selectedFileIndex = index;
						selectedScene = iter->second;
						break;
					}

					++index;
				}
				break;
			case Core::AssetType_Prefab:
				for (map<string, shared_ptr<Prefab>>::iterator iter = m_assetManager->prefabMap.begin();
					iter != m_assetManager->prefabMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), &fileSelections[index]))
					{
						selectedFileIndex = index;
						selectedPrefab = iter->second;
						break;
					}

					++index;
				}
				break;
			case Core::AssetType_GLShader:
				for (map<string, shared_ptr<GLSL>>::iterator iter = m_assetManager->glVertexShaderMap.begin();
					iter != m_assetManager->glVertexShaderMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), &fileSelections[index]))
					{
						selectedFileIndex = index;
						break;
					}

					++index;
				}
				for (map<string, shared_ptr<GLSL>>::iterator iter = m_assetManager->glFragmentShaderMap.begin();
					iter != m_assetManager->glFragmentShaderMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), &fileSelections[index]))
					{
						selectedFileIndex = index;
						break;
					}

					++index;
				}
				break;
			case Core::AssetType_StaticMesh:
				for (map<string, shared_ptr<StaticMesh>>::iterator iter = m_assetManager->staticMeshMap.begin();
					iter != m_assetManager->staticMeshMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), &fileSelections[index]))
					{
						selectedFileIndex = index;
						selectedStaticMesh = iter->second;
						break;
					}

					++index;
				}
				break;
			case Core::AssetType_Texture:
				for (map<string, shared_ptr<Texture>>::iterator iter = m_assetManager->textureMap.begin();
					iter != m_assetManager->textureMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), &fileSelections[index]))
					{
						selectedFileIndex = index;
						selectedTexture = iter->second;
						break;
					}

					++index;
				}
				break;
			case Core::AssetType_Lightmap:
				for (map<string, shared_ptr<Texture>>::iterator iter = m_assetManager->lightmapMap.begin();
					iter != m_assetManager->lightmapMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), &fileSelections[index]))
					{
						selectedFileIndex = index;
						selectedTexture = iter->second;
						break;
					}

					++index;
				}
				break;
			case Core::AssetType_Material:
				for (map<string, shared_ptr<Material>>::iterator iter = m_assetManager->materialMap.begin();
					iter != m_assetManager->materialMap.end();
					++iter)
				{
					if (ImGui::Selectable(iter->second->fileNameWithExt.c_str(), &fileSelections[index]))
					{
						selectedFileIndex = index;
						selectedMaterial = iter->second;
						break;
					}

					++index;
				}
				break;
			default:
				break;
			}

			if (selectedFileIndex != selectedFileLastFrameIndex)
			{
				if (selectedFileLastFrameIndex != InvalidIndex)
					fileSelections[selectedFileLastFrameIndex] = false;
			}
			else
				fileSelections[selectedFileIndex] = true;

			selectedFileLastFrameIndex = selectedFileIndex;

			ImGui::EndChild();
			ImGui::PopStyleVar();

			//////////////////////////////////////////////////////////////////////////
			if (selectedCategory == AssetType_Prefab && selectedFileIndex != InvalidIndex)
			{
				ImGui::NextColumn();

				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild("Inspector", ImVec2(0, 300), true, 0);

				ImGui::Columns(2);

				static int32 radioIndex = 0;

				//	Static Mesh
				if (ImGui::RadioButton("Static Mesh", &radioIndex, 0))
				{
					ImGui::OpenPopup("Static Mesh...");

					memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
					selectedFileIndexInInspector = InvalidIndex;
					selectedFileLastFrameIndexInInspector = InvalidIndex;
				}

				//	Material
				if (ImGui::RadioButton("Material", &radioIndex, 1))
				{
					ImGui::OpenPopup("Material...");

					memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
					selectedFileIndexInInspector = InvalidIndex;
					selectedFileLastFrameIndexInInspector = InvalidIndex;
				}

				//if (ImGui::Button("Add to Scene"))
				//	m_scene->AddObject(createObject(selectedPrefab));

				ImGui::NextColumn();

				//	Static Mesh
				popupFileSelectingTick(
					FileType_StaticMesh,
					"Static Mesh...",
					fileSelectionsInInspector,
					selectedFileIndexInInspector,
					selectedFileLastFrameIndexInInspector,
					inputState,
					pressedOK,
					fileName);

				if (pressedOK)
				{
					selectedPrefab.lock()->staticMeshName = fileName;
					pressedOK = False;
				}

				if (!selectedPrefab.expired())
					ImGui::Button(selectedPrefab.lock()->staticMeshName.c_str());

				//	Material
				popupFileSelectingTick(
					FileType_Material,
					"Material...",
					fileSelectionsInInspector,
					selectedFileIndexInInspector,
					selectedFileLastFrameIndexInInspector,
					inputState,
					pressedOK,
					fileName);

				if (pressedOK)
				{
					selectedPrefab.lock()->materialName = fileName;
					pressedOK = False;
				}

				if (!selectedPrefab.expired())
					ImGui::Button(selectedPrefab.lock()->materialName.c_str());

				ImGui::EndChild();
				ImGui::PopStyleVar();
			}
			if (selectedCategory == AssetType_StaticMesh && selectedFileIndex != InvalidIndex)
			{
				ImGui::NextColumn();

				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild("Inspector", ImVec2(0, 300), true, 0);

				ImGui::Columns(1);

				if (ImGui::Button("Open its scene"))
				{
					if (m_scene)
					{
						m_scene->Clear();
					}
					
					std::string SceneName = selectedStaticMesh.lock()->fileName;
					
					if (m_assetManager->sceneMap.find(SceneName) != m_assetManager->sceneMap.end())
					{
						m_scene = m_assetManager->sceneMap[SceneName];

						InstantiateScene(m_scene.get());
					}
					else
					{
						m_scene = m_assetManager->CreateScene(SceneName.c_str());

						for (map<string, shared_ptr<Prefab>>::iterator iter = m_assetManager->prefabMap.begin();
							iter != m_assetManager->prefabMap.end();
							++iter)
						{
							if (iter->second->fileName == selectedStaticMesh.lock()->fileName)
							{
								m_scene->AddObject(createObject(iter->second));
							}
						}
					}

					if (m_scene->GetBeingBakingObject() && m_scene->GetBeingBakingObject()->glRenderableUnit->material.lock()->lightmapName == "DummyLightmap")
					{
						m_LightmapEncodingInRGBM = False;
					}
					
					m_scene->Initialize();
				}
				
				ImGui::EndChild();
				ImGui::PopStyleVar();
			}
			else if (selectedCategory == AssetType_Material && selectedFileIndex != InvalidIndex)
			{
				ImGui::NextColumn();

				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild("Inspector", ImVec2(0, 230), true, 0);

				ImGui::Columns(2);

				static int32 radioIndex = 0;

				//	Albdeo
				if (ImGui::RadioButton("Albedo", &radioIndex, 0))
				{
					ImGui::OpenPopup("Texture...");

					memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
					selectedFileIndexInInspector = InvalidIndex;
					selectedFileLastFrameIndexInInspector = InvalidIndex;
				}

				//	Normal Map
				// if (ImGui::RadioButton("Normal Map", &radioIndex, 1))
				// {
				// 	ImGui::OpenPopup("Normal Map...");
				//
				// 	memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
				// 	selectedFileIndexInInspector = InvalidIndex;
				// 	selectedFileLastFrameIndexInInspector = InvalidIndex;
				// }

				//	Metallic Map
				// if (ImGui::RadioButton("Metallic Map", &radioIndex, 2))
				// {
				// 	ImGui::OpenPopup("Metallic Map...");
				//
				// 	memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
				// 	selectedFileIndexInInspector = InvalidIndex;
				// 	selectedFileLastFrameIndexInInspector = InvalidIndex;
				// }

				//	Roughness Map
				// if (ImGui::RadioButton("Roughness Map", &radioIndex, 3))
				// {
				// 	ImGui::OpenPopup("Roughness Map...");
				//
				// 	memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
				// 	selectedFileIndexInInspector = InvalidIndex;
				// 	selectedFileLastFrameIndexInInspector = InvalidIndex;
				// }

				//	Lightmap
				if (ImGui::RadioButton("Lightmap", &radioIndex, 4))
				{
					ImGui::OpenPopup("Lightmap...");

					memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
					selectedFileIndexInInspector = InvalidIndex;
					selectedFileLastFrameIndexInInspector = InvalidIndex;
				}

				//	GL Vertex Shader
				// if (ImGui::RadioButton("GL VS", &radioIndex, 5))
				// {
				// 	ImGui::OpenPopup("GL Vertex Shader...");
				//
				// 	memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
				// 	selectedFileIndexInInspector = InvalidIndex;
				// 	selectedFileLastFrameIndexInInspector = InvalidIndex;
				// 	ImGui::SameLine();
				// }

				//	GL Fragment Shader
				// if (ImGui::RadioButton("GL FS", &radioIndex, 6))
				// {
				// 	ImGui::OpenPopup("GL Fragment Shader...");
				//
				// 	memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
				// 	selectedFileIndexInInspector = InvalidIndex;
				// 	selectedFileLastFrameIndexInInspector = InvalidIndex;
				// 	ImGui::SameLine();
				// }

				//	RL Vertex Shader
				// if (ImGui::RadioButton("RL VS", &radioIndex, 7))
				// {
				// 	ImGui::OpenPopup("RL Vertex Shader...");
				//
				// 	memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
				// 	selectedFileIndexInInspector = InvalidIndex;
				// 	selectedFileLastFrameIndexInInspector = InvalidIndex;
				// 	ImGui::SameLine();
				// }

				//	RL Ray Shader
				// if (ImGui::RadioButton("RL RS", &radioIndex, 8))
				// {
				// 	ImGui::OpenPopup("RL Ray Shader...");
				//
				// 	memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
				// 	selectedFileIndexInInspector = InvalidIndex;
				// 	selectedFileLastFrameIndexInInspector = InvalidIndex;
				// 	ImGui::SameLine();
				// }

				ImGui::NextColumn();

				//	Albdeo
				popupFileSelectingTick(
					FileType_Texture,
					"Texture...",
					fileSelectionsInInspector,
					selectedFileIndexInInspector,
					selectedFileLastFrameIndexInInspector,
					inputState,
					pressedOK,
					fileName);

				if (pressedOK)
				{
					selectedMaterial.lock()->albedoTextureName = fileName;
					selectedMaterial.lock()->albedoTexture = m_assetManager->textureMap[fileName];
					selectedMaterial.lock()->albedoTexture.lock()->BeginUse();
					pressedOK = False;
				}

				if (!selectedMaterial.expired())
					ImGui::Button(selectedMaterial.lock()->albedoTextureName.c_str());

				//	Normal Map
				// popupFileSelectingTick(
				// 	FileType_Texture,
				// 	"Normal Map...",
				// 	fileSelectionsInInspector,
				// 	selectedFileIndexInInspector,
				// 	selectedFileLastFrameIndexInInspector,
				// 	inputState,
				// 	pressedOK,
				// 	fileName);
				//
				// if (pressedOK)
				// {
				// 	selectedMaterial.lock()->normalMapName = fileName;
				// 	selectedMaterial.lock()->normalMap = m_assetManager->textureMap[fileName];
				// 	selectedMaterial.lock()->normalMap.lock()->BeginUse();
				// 	pressedOK = False;
				// }
				//
				// if (!selectedMaterial.expired())
				// 	ImGui::Button(selectedMaterial.lock()->normalMapName.c_str());

				//	Metallic Map
				// popupFileSelectingTick(
				// 	FileType_Texture,
				// 	"Metallic Map...",
				// 	fileSelectionsInInspector,
				// 	selectedFileIndexInInspector,
				// 	selectedFileLastFrameIndexInInspector,
				// 	inputState,
				// 	pressedOK,
				// 	fileName);
				//
				// if (pressedOK)
				// {
				// 	selectedMaterial.lock()->metallicTextureName = fileName;
				// 	selectedMaterial.lock()->metallicTexture = m_assetManager->textureMap[fileName];
				// 	selectedMaterial.lock()->metallicTexture.lock()->BeginUse();
				// 	pressedOK = False;
				// }
				//
				// if (!selectedMaterial.expired())
				// 	ImGui::Button(selectedMaterial.lock()->metallicTextureName.c_str());

				//	Roughness Map
				// popupFileSelectingTick(
				// 	FileType_Texture,
				// 	"Roughness Map...",
				// 	fileSelectionsInInspector,
				// 	selectedFileIndexInInspector,
				// 	selectedFileLastFrameIndexInInspector,
				// 	inputState,
				// 	pressedOK,
				// 	fileName);
				//
				// if (pressedOK)
				// {
				// 	selectedMaterial.lock()->roughnessTextureName = fileName;
				// 	selectedMaterial.lock()->roughnessTexture = m_assetManager->textureMap[fileName];
				// 	selectedMaterial.lock()->roughnessTexture.lock()->BeginUse();
				// 	pressedOK = False;
				// }
				//
				// if (!selectedMaterial.expired())
				// 	ImGui::Button(selectedMaterial.lock()->roughnessTextureName.c_str());

				//	Lightmap
				popupFileSelectingTick(
					FileType_Lightmap,
					"Lightmap...",
					fileSelectionsInInspector,
					selectedFileIndexInInspector,
					selectedFileLastFrameIndexInInspector,
					inputState,
					pressedOK,
					fileName);

				if (pressedOK)
				{
					selectedMaterial.lock()->lightmapName = fileName;
					selectedMaterial.lock()->lightmapTexture = m_assetManager->lightmapMap[fileName];
					selectedMaterial.lock()->lightmapTexture.lock()->BeginUse();
					pressedOK = False;
					selectedMaterial.lock()->IsBeingBaking = False;

					if (fileName != "DummyLightmap")
					{
						m_LightmapEncodingInRGBM = True;
					}
				}

				if (!selectedMaterial.expired())
					ImGui::Button(selectedMaterial.lock()->lightmapName.c_str());
				
				//	GL Vertex Shader
				// popupFileSelectingTick(
				// 	FileType_GLSL_Vertex,
				// 	"GL Vertex Shader...",
				// 	fileSelectionsInInspector,
				// 	selectedFileIndexInInspector,
				// 	selectedFileLastFrameIndexInInspector,
				// 	inputState,
				// 	pressedOK,
				// 	fileName);
				//
				// if (pressedOK)
				// {
				// 	selectedMaterial.lock()->glVertexShaderName = fileName;
				// 	selectedMaterial.lock()->glVertexShader = m_assetManager->glVertexShaderMap[fileName];
				// 	pressedOK = False;
				// }
				//
				// if (!selectedMaterial.expired())
				// 	ImGui::Button(selectedMaterial.lock()->glVertexShaderName.c_str());

				//	GL Fragment Shader
				// popupFileSelectingTick(
				// 	FileType_GLSL_Fragment,
				// 	"GL Fragment Shader...",
				// 	fileSelectionsInInspector,
				// 	selectedFileIndexInInspector,
				// 	selectedFileLastFrameIndexInInspector,
				// 	inputState,
				// 	pressedOK,
				// 	fileName);
				//
				// if (pressedOK)
				// {
				// 	selectedMaterial.lock()->glFragmentShaderName = fileName;
				// 	selectedMaterial.lock()->glFragmentShader = m_assetManager->glFragmentShaderMap[fileName];
				// 	pressedOK = False;
				// }
				//
				// if (!selectedMaterial.expired())
				// 	ImGui::Button(selectedMaterial.lock()->glFragmentShaderName.c_str());

				//	RL Vertex Shader
				// popupFileSelectingTick(
				// 	FileType_RLSL_Vertex,
				// 	"RL Vertex Shader...",
				// 	fileSelectionsInInspector,
				// 	selectedFileIndexInInspector,
				// 	selectedFileLastFrameIndexInInspector,
				// 	inputState,
				// 	pressedOK,
				// 	fileName);
				//
				// if (pressedOK)
				// {
				// 	selectedMaterial.lock()->rlVertexShaderName = fileName;
				// 	selectedMaterial.lock()->rlVertexShader = m_assetManager->rlVertexShaderMap[fileName];
				// 	pressedOK = False;
				// }
				//
				// if (!selectedMaterial.expired())
				// 	ImGui::Button(selectedMaterial.lock()->rlVertexShaderName.c_str());

				//	RL Ray Shader
				// popupFileSelectingTick(
				// 	FileType_RLSL_Ray,
				// 	"RL Ray Shader...",
				// 	fileSelectionsInInspector,
				// 	selectedFileIndexInInspector,
				// 	selectedFileLastFrameIndexInInspector,
				// 	inputState,
				// 	pressedOK,
				// 	fileName);
				//
				// if (pressedOK)
				// {
				// 	selectedMaterial.lock()->rlRayShaderName = fileName;
				// 	selectedMaterial.lock()->rlRayShader = m_assetManager->rlRayShaderMap[fileName];
				// 	pressedOK = False;
				// }
				//
				// if (!selectedMaterial.expired())
				// 	ImGui::Button(selectedMaterial.lock()->rlRayShaderName.c_str());
				
				ImGui::EndChild();
				ImGui::PopStyleVar();

				ImGui::BeginChild("Material Panel", ImVec2(0, 50), true, 0);
				//ImGui::ColorEdit4("Albedo Color", selectedMaterial.lock()->albedoColor);
				if (ImGui::RadioButton("Use Runtime Radiosity", selectedMaterial.lock()->IsBeingBaking))
				{
					selectedMaterial.lock()->IsBeingBaking = !selectedMaterial.lock()->IsBeingBaking;

					if (selectedMaterial.lock()->lightmapName != "DummyLightmap")
					{
						m_LightmapEncodingInRGBM = !m_LightmapEncodingInRGBM;
					}
				}
				ImGui::EndChild();
			}
			else if (selectedCategory == AssetType_Texture || selectedCategory == AssetType_Lightmap && selectedFileIndex != InvalidIndex)
			{
				if (!selectedTexture.expired() && selectedTexture.lock()->BeingUsed())
				{
					ImGui::NextColumn();

					ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
					ImGui::BeginChild("Inspector", ImVec2(0, 300), true, 0);

					string wrapModeString = "";

					if (selectedTexture.lock()->info->wrapMode == TextureWrapMode_Clamp)
						wrapModeString = "Clmap";
					else if (selectedTexture.lock()->info->wrapMode == TextureWrapMode_Repeat)
						wrapModeString = "Repeat";

					string filterModeString = "";

					if (selectedTexture.lock()->info->filterMode == TextureFilterMode_Point)
						filterModeString = "Point";
					else if (selectedTexture.lock()->info->filterMode == TextureFilterMode_Bilinear)
						filterModeString = "Bilinear";

					string formatString = "";

					if (selectedTexture.lock()->info->format == TextureFormat_RGB24)
						formatString = "RGB24";
					else if (selectedTexture.lock()->info->format == TextureFormat_RGBA32)
						formatString = "RGBA32";
					else if (selectedTexture.lock()->info->format == TextureFormat_SRGB24)
						formatString = "SRGB24";
					else if (selectedTexture.lock()->info->format == TextureFormat_SRGBA32)
						formatString = "SRGBA32";

					if (ImGui::BeginCombo("Wrap Mode", wrapModeString.c_str()))
					{
						if (ImGui::Selectable("Clmap"))
						{
							selectedTexture.lock()->info->wrapMode = TextureWrapMode_Clamp;
							selectedTexture.lock()->SetGLWrapMode(GLTextureWrapMode_Clamp);
						}

						if (ImGui::Selectable("Repeat"))
						{
							selectedTexture.lock()->info->wrapMode = TextureWrapMode_Repeat;
							selectedTexture.lock()->SetGLWrapMode(GLTextureWrapMode_Repeat);
						}

						ImGui::EndCombo();
					}

					if (ImGui::BeginCombo("Filter Mode", filterModeString.c_str()))
					{
						if (ImGui::Selectable("Point"))
						{
							selectedTexture.lock()->info->filterMode = TextureFilterMode_Point;
							selectedTexture.lock()->SetGLFilterMode(GLTextureFilterMode_Point);
						}

						if (ImGui::Selectable("Bilinear"))
						{
							selectedTexture.lock()->info->filterMode = TextureFilterMode_Bilinear;
							selectedTexture.lock()->SetGLFilterMode(GLTextureFilterMode_Bilinear);
						}

						ImGui::EndCombo();
					}

					bool bypassSRGB = selectedTexture.lock()->info->format != TextureFormat_SRGB24 &&
						selectedTexture.lock()->info->format != TextureFormat_SRGBA32;

					if (ImGui::Checkbox("Bypass SRGB", &bypassSRGB))
					{
						if (selectedTexture.lock()->info->format == TextureFormat_RGB24)
							selectedTexture.lock()->info->format = TextureFormat_SRGB24;
						else if (selectedTexture.lock()->info->format == TextureFormat_SRGB24)
							selectedTexture.lock()->info->format = TextureFormat_RGB24;
						else if (selectedTexture.lock()->info->format == TextureFormat_RGBA32)
							selectedTexture.lock()->info->format = TextureFormat_SRGBA32;
						else if (selectedTexture.lock()->info->format == TextureFormat_SRGBA32)
							selectedTexture.lock()->info->format = TextureFormat_RGBA32;

						selectedTexture.lock()->UploadToGL();
					}

					ImGui::EndChild();
					ImGui::PopStyleVar();
				}
			}
		}
		ImGui::End();
	}

	void WindowsEditor::menuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				bool fileSelected = false;
				ImGui::MenuItem("File", Null, &fileSelected);

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Object"))
			{
				if (ImGui::BeginMenu("Create"))
				{
					ImGui::MenuItem("Direction Light");
					ImGui::MenuItem("Point Light");
	
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void EditTransform(const ImVec2 & regionTopLeft, const ImVec2 & regionSize, const float *cameraView, float *cameraProjection, float* matrix, Object * pSelectedObject, Bool IsBaking)
	{
		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
		static bool useSnap = false;
		static float snap[3] = { 1.f, 1.f, 1.f };

		if (ImGui::IsKeyPressed(90))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(69))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(82)) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation, 3);
		ImGui::InputFloat3("Rt", matrixRotation, 3);
		ImGui::InputFloat3("Sc", matrixScale, 3);
		
		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}
		if (ImGui::IsKeyPressed(83))
			useSnap = !useSnap;
		ImGui::Checkbox("", &useSnap);
		ImGui::SameLine();

		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
		}

		if (pSelectedObject->IsLight)
		{
			ImGui::ColorEdit3("Light Color", pSelectedObject->Color);
			ImGui::InputFloat("Intensity(cd/m^2)", &pSelectedObject->Intensity);
			pSelectedObject->Energy[0] = pSelectedObject->Color[0] * pSelectedObject->Intensity;
			pSelectedObject->Energy[1] = pSelectedObject->Color[1] * pSelectedObject->Intensity;
			pSelectedObject->Energy[2] = pSelectedObject->Color[2] * pSelectedObject->Intensity;

			if (ImGui::BeginCombo("Light Precision", pSelectedObject->PrecisionString))
			{
				for (int32 i = LightPrecision_Invalid + 1; i < LightPrecision_Count; ++i)
				{
					Bool IsSelected = (pSelectedObject->LightPrecision == i);

					if (ImGui::Selectable(LightPrecisionItems[i], IsSelected))
					{
						pSelectedObject->LightPrecision = static_cast<LightPrecision>(i);
						pSelectedObject->PrecisionString = LightPrecisionItems[i];
					}
					
					if (IsSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			int32 MinX = GetMinSizeLength(pSelectedObject->LightPrecision);
			int32 MinY = GetMinSizeLength(pSelectedObject->LightPrecision);

			if (pSelectedObject->XLength < MinX)
			{
				pSelectedObject->XLength = MinX;
			}

			if (pSelectedObject->YLength < MinY)
			{
				pSelectedObject->YLength = MinY;
			}

			ImGui::SliderInt("XLength", &pSelectedObject->XLength, MinX, pSelectedObject->MaxXLength);
			ImGui::SameLine();
			ImGui::InputInt("MaxX", &pSelectedObject->MaxXLength);

			ImGui::SliderInt("YLength", &pSelectedObject->YLength, MinY, pSelectedObject->MaxYLength);
			ImGui::SameLine();
			ImGui::InputInt("MaxY", &pSelectedObject->MaxYLength);
			
			if (pSelectedObject->XLength % MinX > 0)
			{
				pSelectedObject->XLength -= pSelectedObject->XLength % MinX;
				pSelectedObject->XLength += MinX;
			}

			if (pSelectedObject->YLength % MinY > 0)
			{
				pSelectedObject->YLength -= pSelectedObject->YLength % MinY;
				pSelectedObject->YLength += MinY;
			}

			//	We don't update matrixScale when baking.
			if (!IsBaking)
			{
				matrixScale[0] = static_cast<float>(pSelectedObject->XLength);
				matrixScale[1] = static_cast<float>(pSelectedObject->YLength);
			}

			ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);
		}
		else
		{
			if (ImGui::BeginCombo("Lightmap Resolution", pSelectedObject->ResolutionString))
			{
				for (int32 i = LightmapResolution_Invalid + 1; i < LightmapResolution_Count; ++i)
				{
					Bool IsSelected = (pSelectedObject->LightmapResolution == i);

					if (ImGui::Selectable(LightmapResolutionItems[i], IsSelected))
					{
						pSelectedObject->LightmapResolution = static_cast<LightmapResolution>(i);
						pSelectedObject->ResolutionString = LightmapResolutionItems[i];
					}
				
					if (IsSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
		
		ImGui::End();

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(regionTopLeft.x, regionTopLeft.y, regionSize.x, regionSize.y);
		ImGuizmo::SetDrawlist();
		ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL);
	}

	void WindowsEditor::guizmoTick(const ImVec2 & regionTopLeft, const ImVec2 & regionSize, void * pViewMatrix, void * pProjectionMatrix, Object * pSelectedObject)
	{
		ImGuizmo::BeginFrame(regionSize);

		ImGui::Begin("Inspector");
		
		EditTransform(regionTopLeft, regionSize, (float*)pViewMatrix, (float *)pProjectionMatrix, (float *)pSelectedObject->GetObject2WorldMatrix(), pSelectedObject, m_baking);
	}

	std::shared_ptr<Object> WindowsEditor::createObject(std::weak_ptr<Prefab> prefab)
	{
		std::shared_ptr<Object> defaultObject = std::make_shared<Object>();

		defaultObject->name = prefab.lock()->fileName;
		defaultObject->staticMeshName = prefab.lock()->staticMeshName;
		defaultObject->materialName = prefab.lock()->materialName;

		defaultObject->glRenderableUnit = std::make_unique<GLRenderableUnit>();
		defaultObject->glRenderableUnit->staticMesh = m_assetManager->staticMeshMap[prefab.lock()->staticMeshName];

		std::shared_ptr<Material> material = m_assetManager->materialMap[prefab.lock()->materialName];

		material->lightmapName = "DummyLightmap";
		if (m_assetManager->lightmapMap.find("DummyLightmap") != m_assetManager->lightmapMap.end())
		{
			material->lightmapTexture = m_assetManager->lightmapMap["DummyLightmap"];
			material->lightmapTexture.lock()->Attach(material.get());
		}

		material->IsOccluder = True;

		defaultObject->glRenderableUnit->material = material;
		defaultObject->glRenderableUnit->DrawGBufferMaterial = m_DrawGBufferMaterial;
		defaultObject->glRenderableUnit->DrawIDMaterial = m_DrawIDMaterial;
		defaultObject->glRenderableUnit->ComputeFormFactorMaterial = m_ComputeFormFactorMaterial;

		defaultObject->rlRenderableUnit = std::make_shared<RLRenderableUnit>();
		defaultObject->rlRenderableUnit->staticMesh = m_assetManager->staticMeshMap[prefab.lock()->staticMeshName];
		defaultObject->rlRenderableUnit->material = material;
		material->rlVertexShader.lock()->Attach(defaultObject->rlRenderableUnit.get());
		material->rlRayShader.lock()->Attach(defaultObject->rlRenderableUnit.get());

		defaultObject->LightmapResolution = LightmapResolution_256x256;
		defaultObject->IsLight = False;
		
		defaultObject->Initialize(m_GLDevice.get(), False);

		return defaultObject;
	}

	std::shared_ptr<Object> WindowsEditor::InstantiateObject(std::shared_ptr<Object> object)
	{
		object->glRenderableUnit = std::make_unique<GLRenderableUnit>();
		object->glRenderableUnit->staticMesh = m_assetManager->staticMeshMap[object->staticMeshName];

		std::shared_ptr<Material> material = m_assetManager->materialMap[object->materialName];

		material->lightmapName = "DummyLightmap";
		if (m_assetManager->lightmapMap.find("DummyLightmap") != m_assetManager->lightmapMap.end())
		{
			material->lightmapTexture = m_assetManager->lightmapMap["DummyLightmap"];
			material->lightmapTexture.lock()->Attach(material.get());
		}

		material->IsOccluder = True;

		object->glRenderableUnit->material = material;
		object->glRenderableUnit->DrawGBufferMaterial = m_DrawGBufferMaterial;
		object->glRenderableUnit->DrawIDMaterial = m_DrawIDMaterial;
		object->glRenderableUnit->ComputeFormFactorMaterial = m_ComputeFormFactorMaterial;

		object->rlRenderableUnit = std::make_shared<RLRenderableUnit>();
		object->rlRenderableUnit->staticMesh = m_assetManager->staticMeshMap[object->staticMeshName];
		object->rlRenderableUnit->material = material;
		material->rlVertexShader.lock()->Attach(object->rlRenderableUnit.get());
		material->rlRayShader.lock()->Attach(object->rlRenderableUnit.get());

		object->name = object->staticMeshName;

		object->Initialize(m_GLDevice.get(), True);

		return object;
	}

	std::shared_ptr<Object> WindowsEditor::InstantiateAreaLight(std::shared_ptr<Object> object)
	{
		object->glRenderableUnit = std::make_unique<GLRenderableUnit>();
		object->glRenderableUnit->staticMesh = m_DefaultLightMesh;// CreateAreaLightMesh(object->scale.x, object->scale.y, object->LightPrecision);
		object->glRenderableUnit->material = m_arealLightMaterial;
		
		object->Initialize(m_GLDevice.get(), True);

		return object;
	}
	
	std::shared_ptr<Object> WindowsEditor::createAreaLight(int32 Index)
	{
		std::shared_ptr<Object> areaLight = std::make_shared<Object>();
		ctd::string name = "Area Light " + to_string(Index);
		areaLight->name = name;

		areaLight->glRenderableUnit = std::make_unique<GLRenderableUnit>();
		
		float DefaultXLength = 100.0f;
		float DefaultYLength = 100.0f;
		areaLight->glRenderableUnit->staticMesh = m_DefaultLightMesh;// CreateAreaLightMesh(DefaultXLength, DefaultYLength, LightPrecision_Middle);
		areaLight->scale.x = DefaultXLength;
		areaLight->scale.y = DefaultYLength;
		areaLight->XLength = static_cast<int32>(DefaultXLength);
		areaLight->YLength = static_cast<int32>(DefaultYLength);

		areaLight->glRenderableUnit->material = m_arealLightMaterial;
		
		areaLight->position = Vector3(0, 0, 0);
		areaLight->eulerAngle = Vector3(-90.0f, 0, 0);
		//	Should not be exported to editor panel.
		areaLight->LightmapResolution = LightmapResolution_Invalid;
		areaLight->LightPrecision = LightPrecision_Low;
		areaLight->IsLight = True;

		return areaLight;
	}

	std::unique_ptr<Object> WindowsEditor::CreateObject(const Primitive& Primitive)
	{
		shared_ptr<StaticMesh> PrimitiveMesh = std::make_shared<StaticMesh>();
		PrimitiveMesh->pPositions = new Vector4[3];
		PrimitiveMesh->vertexCount = 3;

		PrimitiveMesh->pPositions[0] = Primitive.Positions[0];
		PrimitiveMesh->pPositions[1] = Primitive.Positions[1];
		PrimitiveMesh->pPositions[2] = Primitive.Positions[2];

		//	offset for raycast testing.
		Vector3 Offset = Primitive.Normal * Vector3(0.1f);
		
		PrimitiveMesh->pPositions[0].x += Offset.x;
		PrimitiveMesh->pPositions[0].y += Offset.y;
		PrimitiveMesh->pPositions[0].z += Offset.z;
		
		PrimitiveMesh->pPositions[1].x += Offset.x;
		PrimitiveMesh->pPositions[1].y += Offset.y;
		PrimitiveMesh->pPositions[1].z += Offset.z;

		PrimitiveMesh->pPositions[2].x += Offset.x;
		PrimitiveMesh->pPositions[2].y += Offset.y;
		PrimitiveMesh->pPositions[2].z += Offset.z;
		
		PrimitiveMesh->pIndices = new uint32[3];
		PrimitiveMesh->indexCount = 3;
		PrimitiveMesh->pIndices[0] = 0;
		PrimitiveMesh->pIndices[1] = 1;
		PrimitiveMesh->pIndices[2] = 2;

		shared_ptr<Material> PrimitiveMaterial = make_shared<Material>();
		PrimitiveMaterial->IsOccluder = True;	//	TODO:	�����û��¶�ڱ༭����
		PrimitiveMaterial->rlVertexShader = m_assetManager->rlVertexShaderMap["primitive"];
		PrimitiveMaterial->rlRayShader = m_assetManager->rlRayShaderMap["primitive"];
		
		std::unique_ptr<Object> PrimitiveObject = std::make_unique<Object>();
		PrimitiveObject->rlRenderableUnit = std::make_shared<RLRenderableUnit>();
		PrimitiveObject->rlRenderableUnit->staticMesh = PrimitiveMesh;
		PrimitiveObject->rlRenderableUnit->material = PrimitiveMaterial;

		//	Primitive Object will only being existing in one frame, so we disable its shader reload.
		//PrimitiveMaterial->rlVertexShader.lock()->Attach(PrimitiveObject->rlRenderableUnit.get());
		//PrimitiveMaterial->rlRayShader.lock()->Attach(PrimitiveObject->rlRenderableUnit.get());

		PrimitiveMesh->BeginUse();
		PrimitiveObject->rlRenderableUnit->Commit();

		return PrimitiveObject;
	}
	
	void WindowsEditor::SaveLightmap(std::string Name, float* RadiosityImageRawData, int32 Width, int32 Height)
	{
		uint8* LightmapRawData = new uint8[Width * Height * sizeof(uint8) * 4];

		for (int32 i = 0; i < Height; ++i)
		{
			for (int32 j = 0; j < Width; ++j)
			{
				float R = RadiosityImageRawData[(i * Width + j) * 4 + 0];
				float G = RadiosityImageRawData[(i * Width + j) * 4 + 1];
				float B = RadiosityImageRawData[(i * Width + j) * 4 + 2];
				float A = 1.0f;

				LinearColor HDR = LinearColor(R, G, B, A);
				Color RGBM = HDR.ToRGBM();

				LightmapRawData[(i * Width + j) * 4 + 0] = RGBM.R;
				LightmapRawData[(i * Width + j) * 4 + 1] = RGBM.G;
				LightmapRawData[(i * Width + j) * 4 + 2] = RGBM.B;
				LightmapRawData[(i * Width + j) * 4 + 3] = RGBM.A;
			}
		}
		
		m_assetManager->SaveLightmap(
			Name.c_str(),
			TextureFormat_RGBA32,
			LightmapRawData,
			Width,
			Height
			);

		delete[] LightmapRawData;
	}

	void CalculateOffsetedCoordinate(int32 Direction, int32 Width, int32 Height, int32 X, int32 Y, int32& OffsetedX, int32& OffsetedY)
	{
		switch (Direction)
		{
		case 0:
			OffsetedX = X - 1;
			OffsetedY = Y;
			break ;
		case 1:
			OffsetedX = X + 1;
			OffsetedY = Y;
			break;
		case 2:
			OffsetedX = X;
			OffsetedY = Y + 1;
			break;
		case 3:
			OffsetedX = X;
			OffsetedY = Y - 1;
			break ;
		case 4:
			OffsetedX = X - 1;
			OffsetedY = Y + 1;
			break;
		case 5:
			OffsetedX = X + 1;
			OffsetedY = Y + 1;
			break;
		case 6:
			OffsetedX = X + 1;
			OffsetedY = Y - 1;
			break;
		case 7:
			OffsetedX = X - 1;
			OffsetedY = Y - 1;
			break ;
		default:
			OffsetedX = 0;
			OffsetedY = 0 ;
			break ;
		}
	}

	void WindowsEditor::DilateLightmap(float* RadiosityImageRawData, int32 Width, int32 Height, float* OutRadiosityImageData)
	{
		Vector2 Offsets[8] = {
			Vector2(-1.0f, 0),
			Vector2(1.0f, 0),
			Vector2(0, 1.0f),
			Vector2(0, -1.0f),
			Vector2(-1.0f, 1.0f),
			Vector2(1.0f, 1.0f),
			Vector2(1.0f, -1.0f),
			Vector2(-1.0f, -1.0f),
		};
		
		for (int32 Y = 0; Y < Height; ++Y)
		{
			for (int32 X = 0; X < Width; ++X)
			{
				//	As a fallback ahead.
				OutRadiosityImageData[(Y * Height + X) * 4 + 0] = RadiosityImageRawData[(Y * Height + X) * 4 + 0];
				OutRadiosityImageData[(Y * Height + X) * 4 + 1] = RadiosityImageRawData[(Y * Height + X) * 4 + 1];
				OutRadiosityImageData[(Y * Height + X) * 4 + 2] = RadiosityImageRawData[(Y * Height + X) * 4 + 2];
				OutRadiosityImageData[(Y * Height + X) * 4 + 3] = RadiosityImageRawData[(Y * Height + X) * 4 + 3];

				//	gutter pixle, need dilating.
				if (RadiosityImageRawData[(Y * Height + X) * 4 + 0] == 0 &&
					RadiosityImageRawData[(Y * Height + X) * 4 + 1] == 0 &&
					RadiosityImageRawData[(Y * Height + X) * 4 + 2] == 0)
				{
					Vector2 UV = Vector2(X, Y);
					float MinDistance = 10.0f;
					
					for (int32 Direction = 0; Direction < 8; ++Direction)
					{
						int32 OffsetedX;
						int32 OffsetedY;
						CalculateOffsetedCoordinate(Direction, Width, Height, X, Y, OffsetedX, OffsetedY);

						if (OffsetedX < 0 || OffsetedX >= Width || OffsetedY < 0 || OffsetedY >= Height)
						{
							continue;
						}
						
						Vector3 OffsetedRadiosity = Vector3(
							RadiosityImageRawData[(OffsetedY * Height + OffsetedX) * 4 + 0],
							RadiosityImageRawData[(OffsetedY * Height + OffsetedX) * 4 + 1],
							RadiosityImageRawData[(OffsetedY * Height + OffsetedX) * 4 + 2]);
						
						Vector2 OffsetedUV = Vector2(X, Y) + Offsets[Direction];
						
						if (RadiosityImageRawData[(OffsetedY * Height + OffsetedX) * 4 + 0] > 0 ||
							RadiosityImageRawData[(OffsetedY * Height + OffsetedX) * 4 + 1] > 0 ||
							RadiosityImageRawData[(OffsetedY * Height + OffsetedX) * 4 + 2] > 0)
						{
							float Distance = Length(UV - OffsetedUV);

							if (Distance < MinDistance)
							{
								int32 ProjectedX;
								int32 ProjectedY;
								CalculateOffsetedCoordinate(Direction, Width, Height, OffsetedX, OffsetedY, ProjectedX, ProjectedY);

								if (ProjectedX < 0 || ProjectedX >= Width || ProjectedY < 0 || ProjectedY >= Height)
								{
									continue;
								}
								
								Vector3 ProjectedRadiosity = Vector3(
									RadiosityImageRawData[(ProjectedY * Height + ProjectedX) * 4 + 0],
									RadiosityImageRawData[(ProjectedY * Height + ProjectedX) * 4 + 1],
									RadiosityImageRawData[(ProjectedY * Height + ProjectedX) * 4 + 2]);
								
								if (RadiosityImageRawData[(ProjectedY * Height + ProjectedX) * 4 + 0] > 0 ||
									RadiosityImageRawData[(ProjectedY * Height + ProjectedX) * 4 + 0] > 0 ||
									RadiosityImageRawData[(ProjectedY * Height + ProjectedX) * 4 + 0] > 0)
								{
									Vector3 Delta = OffsetedRadiosity - ProjectedRadiosity;
									
									OutRadiosityImageData[(Y * Height + X) * 4 + 0] = OffsetedRadiosity.r + Delta.r;
									OutRadiosityImageData[(Y * Height + X) * 4 + 1] = OffsetedRadiosity.g + Delta.g;
									OutRadiosityImageData[(Y * Height + X) * 4 + 2] = OffsetedRadiosity.b + Delta.b;
									OutRadiosityImageData[(Y * Height + X) * 4 + 3] = 1.0f;
								}
								else
								{
									OutRadiosityImageData[(Y * Height + X) * 4 + 0] = OffsetedRadiosity.r;
									OutRadiosityImageData[(Y * Height + X) * 4 + 1] = OffsetedRadiosity.g;
									OutRadiosityImageData[(Y * Height + X) * 4 + 2] = OffsetedRadiosity.b;
									OutRadiosityImageData[(Y * Height + X) * 4 + 3] = 1.0f;
								}
								
								MinDistance = Distance;
							}
						}
					}
				}
			}
		}
	}
	
	void WindowsEditor::InstantiateScene(Scene* CurrentScene)
	{
		//	ʵ�������������л��õĶ���
		for (vector<std::shared_ptr<Object>>::iterator iter = CurrentScene->serializedObjects.begin();
			iter != CurrentScene->serializedObjects.end();
			++iter)
		{
			if ((*iter)->IsLight)
			{
				CurrentScene->AddLight(InstantiateAreaLight(*iter), False);
			}
			else
			{
				CurrentScene->AddObject(InstantiateObject(*iter), False);
			}
		}
	}

	void WindowsEditor::RestoreLightMesh()
	{
		for (int32 i = 0; i < m_scene->GetLightCount(); ++i)
		{
			Object* AreaLight = m_scene->GetAreaLight(i);

			AreaLight->glRenderableUnit->staticMesh = m_DefaultLightMesh;
			AreaLight->scale.x = static_cast<float>(AreaLight->XLength);
			AreaLight->scale.y = static_cast<float>(AreaLight->YLength);
			
			AreaLight->UpdateTransformMatrix();
		}
	}
	
	uint32 WindowsEditor::ReverseBits(uint32 Value)
	{
		Value = (Value << 16u) | (Value >> 16u); 
		Value = ((Value & 0x55555555) << 1u) | ((Value & 0xAAAAAAAA) >> 1u);
		Value = ((Value & 0x33333333) << 2u) | ((Value & 0xCCCCCCCC) >> 2u);
		Value = ((Value & 0x0F0F0F0F) << 4u) | ((Value & 0xF0F0F0F0) >> 4u);
		Value = ((Value & 0x00FF00FF) << 8u) | ((Value & 0xFF00FF00) >> 8u);
		
		return  float(Value) * 2.3283064365386963e-10;
	}

	Vector2 WindowsEditor::Hammersley(uint32 Index, uint32 NumSamples)
	{
		return Vector2(float(Index) / float(NumSamples), ReverseBits(Index));
	}

	Vector3 WindowsEditor::UniformSampleHemisphere(float u, float v)
	{
		float phi = v * 2.0f * PI;
		float cosTheta = 1.0f - u;
		float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
		
		return Vector3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
	}
	
	WindowsEditor::WindowsEditor()
		:
		m_assetManager(std::make_unique<AssetManager>()),
		m_GLDevice(std::make_unique<OpenGLDevice>()),
		m_RLDevice(std::make_unique<RLDevice>()),
		m_guiWrapper(std::make_unique<GUIWrapper>()),
		m_GLFrameBuffer(std::make_unique<GLFrameBuffer>()),
		m_RLBakingObjectPosition(std::make_unique<RLTexture2D>(RLIinternalFormat_RGBA, RLPixelFormat_RGBA, RLDataType_Float, RLTextureWrapMode_Clamp, RLTextureFilterMode_Point)),
		m_RLBakingObjectNormal(std::make_unique<RLTexture2D>(RLIinternalFormat_RGBA, RLPixelFormat_RGBA, RLDataType_Float, RLTextureWrapMode_Clamp, RLTextureFilterMode_Point)),
		m_RLBakeFrameBuffer(std::make_unique<RLFrameBuffer>()),
		m_RLBakeColorAttach(std::make_unique<RLTexture2D>(RLIinternalFormat_RGBA, RLPixelFormat_RGBA, RLDataType_Float, RLTextureWrapMode_Clamp, RLTextureFilterMode_Point)),
		m_RLBakePackingBuffer(std::make_unique<RLBuffer>(RLBufferTarget_PixelPackBuffer)),
		m_GLVisibilityTexture(std::make_shared<GLTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Bilinear)),
		m_frameCount(0),
		m_LightmapEncodingInRGBM(False),
		m_baking(False),
		m_thresholdY(0.01f)
	{
		//	Visibility Pass
		m_primitiveIDCubeMap = std::make_unique<GLTexture>(GLTextureTarget_CubeMAP, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
		m_primitiveIDCubeMap->LoadImage(
		PrimitiveIDTextureWidth,
		PrimitiveIDTextureHeight,
		Null);

		m_visibilityPassFrameBuffer = std::make_unique<GLFrameBuffer>();
		m_visibilityPassFrameBuffer->Resize(PrimitiveIDTextureWidth, PrimitiveIDTextureHeight);

		//	http://www.brucelindbloom.com/index.html?Eqn_RGB_to_XYZ.html
		AdobeRGBD65RGBToXYZ = Matrix3x3(
		0.5767309f,  0.1855540f,  0.1881852f,
		0.2973769f,  0.6273491f,  0.0752741f,
		0.02703434,  0.0706872f,  0.9911085f
		);

		AdobeRGBD65XYZToRGB = Matrix3x3(
		2.0413690f, -0.5649464f, -0.3446944f,
		-0.9692660f,  1.8760108f,  0.0415560f,
		0.0134474f, -0.1183897f,  1.0154096f
		);

		float* Sequences = new float[SamplerCount * 4];
		for (int32 i = 0; i < SamplerCount; ++i)
		{
			Vector2 H = Hammersley(i, SamplerCount);
				
			Sequences[i * 4 + 0] = H.x;
			Sequences[i * 4 + 1] = H.y;
			Sequences[i * 4 + 2] = 0;
			Sequences[i * 4 + 3] = 1.0;
		}
		
		m_RLHammersleyTexture = std::make_unique<RLTexture2D>(RLIinternalFormat_RGBA, RLPixelFormat_RGBA, RLDataType_Float, RLTextureWrapMode_Clamp, RLTextureFilterMode_Point);
		m_RLHammersleyTexture->LoadImage(SamplerCount, 1.0, Sequences);
		delete[] Sequences;
	}

	void WindowsEditor::Initialize(int32 width, int32 height)
	{
		m_assetManager->Scan();

		m_RLDevice->Initialize(
			m_assetManager->rlVertexShaderMap["environment"],
			m_assetManager->rlRayShaderMap["environment"],
			m_assetManager->rlFrameShaderMap["bake"]);

		createBuiltinResources();
		
		m_guiWrapper->Initialize(width, height);

		m_rlShootingPrimitiveBuffer = std::make_unique<RLBuffer>(RLBufferTarget_UniformBlockBuffer);
		m_rlShootingPrimitiveBuffer->name = "ShootingPrimitive";
		m_rlShootingPrimitiveBuffer->AllocateMemorySpace(sizeof(RLShootingPrimitive), RLBufferUsage_DynamicDraw);

		if (m_assetManager->sceneMap.find(startSceneName) != m_assetManager->sceneMap.end())
			m_scene = m_assetManager->sceneMap[startSceneName];
		else
			m_scene = std::make_shared<Scene>();
		
		InstantiateScene(m_scene.get());
		m_scene->Initialize();
	}

	void WindowsEditor::Tick(float deltaTime, int32 width, int32 height, InputState & inputState)
	{
		m_guiWrapper->Tick(width, height, inputState);

		//	Main Window

		//ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;

		//bool mainOpened = true;

		ImGui::SetNextWindowPos(ImVec2(400, 800));
		ImGui::SetNextWindowSize(ImVec2(200, 200));

		ImGui::Begin("Main Window");//, &mainOpened, windowFlags);
		//menuBar();
		if (ImGui::Button("GLSL Reload"))
		{
			m_assetManager->ReloadGLShader();
		}
		ImGui::SameLine();
		if (ImGui::Button("RLSL Reload"))
		{
			m_assetManager->ReloadRLShader();
		}
		
		if (ImGui::Button("Create Light"))
		{
			int32 Index = m_scene->GetLightCount();
			std::shared_ptr<Object> lightObject = createAreaLight(Index);
			lightObject->Initialize(m_GLDevice.get(), True);
			m_scene->AddLight(lightObject, True);
		}

		ImGui::SameLine();

		if (ImGui::Button("Delete Light"))
		{
			if (m_pSelectedObject && m_pSelectedObject->IsLight)
			{
				m_scene->RemoveLight(m_pSelectedObject);
				m_pSelectedObject = Null;
			}
		}
		
		if (m_baking && ImGui::Button("Stop"))
		{
			m_baking = False;
			m_currentMaxY = 0;
			
			RestoreLightMesh();
			
			while (!RemainingPrimitives.empty())
			{
				RemainingPrimitives.pop();
			}
		}
		else if (!m_baking && ImGui::Button("Bake"))
		{
			m_baking = True;
		}
		
		ImGui::SameLine();

		if (m_LightmapEncodingInRGBM)
		{
			ImGui::Text("RGBM");
		}
		else
		{
			ImGui::Text("Linear");
		}
		
		ImGui::SliderFloat("Threshold: ", &m_thresholdY, 0.0001f, 0.05f, "%.5f");
		std::string YString = "Current Y: ";
		YString += to_string(m_currentMaxY);
		ImGui::Text(YString.c_str());
		
		string BakingInfo = "Frame: ";
		BakingInfo += to_string(m_frameCount - 1);
		ImGui::Text(BakingInfo.c_str());
		
		if (ImGui::Button("Save Lightmap"))
		{
			Object* BeingBakingObject = m_scene->GetBeingBakingObject();

			if ((m_frameCount - 1) % 2 == 0)
			{
				m_RadiosityImage1->Fetch(m_pRadiosityImageRawData);
			}
			else
			{
				m_RadiosityImage0->Fetch(m_pRadiosityImageRawData);
			}

			int32 RadiosityTextureWidth = BeingBakingObject->glRenderableUnit->staticMesh->GetRadiosityTextureWidth();
			int32 RadiosityTextureHeight = BeingBakingObject->glRenderableUnit->staticMesh->GetRadiosityTextureHeight();

			string LightmapName = BeingBakingObject->name;
			SaveLightmap(LightmapName, m_pRadiosityImageRawData, RadiosityTextureWidth, RadiosityTextureHeight);
			BeingBakingObject->glRenderableUnit->material.lock()->lightmapName = LightmapName;
			
			float* DilatedRadiosityRawData = new float[RadiosityTextureWidth * RadiosityTextureHeight * 4];
			DilateLightmap(m_pRadiosityImageRawData, RadiosityTextureWidth, RadiosityTextureHeight, DilatedRadiosityRawData);
			
			LightmapName += "_D";
			SaveLightmap(LightmapName, DilatedRadiosityRawData, RadiosityTextureWidth, RadiosityTextureHeight);
			delete[] DilatedRadiosityRawData;
			
			m_assetManager->ScanLightmap();
			m_assetManager->ReloadLightmap();
		}

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(600, 600));

		ImGui::Begin("Rasterized View");
		ImVec2 rasterizedRegion = ImGui::GetContentRegionAvail();
		ImVec2 rasterizedTopLeft = ImGui::GetWindowPos();

		int32 rasterizedViewWidth = static_cast<int32>(rasterizedRegion.x);
		int32 rasterizedViewHeight = static_cast<int32>(rasterizedRegion.y);

		if (rasterizedViewWidth != m_GLFrameBuffer->GetWidth() ||
			rasterizedViewHeight != m_GLFrameBuffer->GetHeight())
		{
			if (rasterizedViewWidth <= 0)
				rasterizedViewWidth = 128;

			if (rasterizedViewHeight <= 0)
				rasterizedViewHeight = 128;

			m_GLColorAttach = std::make_unique<GLTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
			m_GLColorAttach->LoadImage(
				rasterizedViewWidth,
				rasterizedViewHeight,
				Null);

			m_GLFrameBuffer->Resize(rasterizedViewWidth, rasterizedViewHeight);
			m_GLFrameBuffer->AttachColor(GLAttachIndexColor0, m_GLColorAttach->GetTarget(), m_GLColorAttach.get());

			m_scene->GetCamera()->ascept = rasterizedRegion.x / rasterizedRegion.y;
			m_scene->GetCamera()->UpdatePerspectiveProjectionMatrix();
		}
		else
		{
			//	Open new scene, new camera created.
			if (m_scene->GetCamera()->ascept == 0.0)
			{
				m_scene->GetCamera()->ascept = rasterizedRegion.x / rasterizedRegion.y;
				m_scene->GetCamera()->UpdatePerspectiveProjectionMatrix();
			}
		}

		ImGui::Image(
			reinterpret_cast<void *>(m_GLColorAttach->GetID64()),
			rasterizedRegion,
			ImVec2(0, 1.0f),
			ImVec2(1.0f, 0));

		if (m_pSelectedObject != Null)
		{
			guizmoTick(
				rasterizedTopLeft,
				rasterizedRegion,
				m_scene->GetCamera()->GetViewMatrix(),
				m_scene->GetCamera()->GetPerspectiveProjectionMatrix(),
				m_pSelectedObject);
		}

		ImGui::End();
		
		//////////////////////////////////////////////////////////////////////////
		ImGui::SetNextWindowPos(ImVec2(600, 0));
		ImGui::SetNextWindowSize(ImVec2(600, 600));

		ImGui::Begin("Debug View");
		ImVec2 DebugViewRegion = ImGui::GetContentRegionAvail();

		if (m_GLPositionAttach)
		{
			ImGui::Image(
				reinterpret_cast<void *>(m_GLPositionAttach->GetID64()),
			DebugViewRegion,
			ImVec2(0, 1.0f),
			ImVec2(1.0f, 0));
		}
		ImGui::End();
		//////////////////////////////////////////////////////////////////////////
		if (m_GLVisibilityTexture.get())
		{
			ImGui::SetNextWindowPos(ImVec2(1200, 0));
			ImGui::SetNextWindowSize(ImVec2(600, 600));

			ImGui::Begin("Visibility View");
			ImVec2 ProfileViewRegion = ImGui::GetContentRegionAvail();
		
			//ImGui::Image(
			//	reinterpret_cast<void *>(
			//	(m_frameCount - 1) % 2 == 0 ? m_RadiosityImage1->GetID64() : m_RadiosityImage0->GetID64()),
			//	ProfileViewRegion,
			//	ImVec2(0, 1.0f),
			//	ImVec2(1.0f, 0));

			ImGui::Image(
				reinterpret_cast<void *>(m_GLVisibilityTexture->GetID64()),
				ProfileViewRegion,
				ImVec2(0, 1.0f),
				ImVec2(1.0f, 0));
			
			ImGui::End();
		}
		//////////////////////////////////////////////////////////////////////////
		if (m_RadiosityImage0.get())
		{
			ImGui::SetNextWindowPos(ImVec2(0, 600));
			ImGui::SetNextWindowSize(ImVec2(200, 200));
		
			ImGui::Begin("Radiosity 0");
			ImVec2 colorAttach0Region = ImGui::GetContentRegionAvail();
		
			ImGui::Image(
				reinterpret_cast<void *>(m_RadiosityImage0->GetID64()),
				colorAttach0Region,
				ImVec2(0, 1.0f),
				ImVec2(1.0f, 0));
		
			ImGui::End();
		}
		//////////////////////////////////////////////////////////////////////////
		if (m_ResidualImage0.get())
		{
			ImGui::SetNextWindowPos(ImVec2(200, 600));
			ImGui::SetNextWindowSize(ImVec2(200, 200));
		
			ImGui::Begin("Residual 0");
			ImVec2 colorAttach1Region = ImGui::GetContentRegionAvail();
		
			ImGui::Image(
				reinterpret_cast<void *>(m_ResidualImage0->GetID64()),
				colorAttach1Region,
				ImVec2(0, 1.0f),
				ImVec2(1.0f, 0));
		
			ImGui::End();
		}
		//////////////////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////////////////////////////////
		if (m_RadiosityImage1.get())
		{
			ImGui::SetNextWindowPos(ImVec2(0, 800));
			ImGui::SetNextWindowSize(ImVec2(200, 200));
		
			ImGui::Begin("Radiosity 1");
			ImVec2 bakeViewRegion = ImGui::GetContentRegionAvail();
		
			ImGui::Image(
				reinterpret_cast<void *>(m_RadiosityImage1->GetID64()),
				bakeViewRegion,
				ImVec2(0, 1.0f),
				ImVec2(1.0f, 0));
		
			ImGui::End();
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		if (m_ResidualImage1.get())
		{
			ImGui::SetNextWindowPos(ImVec2(200, 800));
			ImGui::SetNextWindowSize(ImVec2(200, 200));

			ImGui::Begin("Residual 1");
			ImVec2 debugViewRegion = ImGui::GetContentRegionAvail();

			ImGui::Image(
				reinterpret_cast<void *>(m_ResidualImage1->GetID64()),
				debugViewRegion,
				ImVec2(0, 1.0f),
				ImVec2(1.0f, 0));

			ImGui::End();
		}
		//////////////////////////////////////////////////////////////////////////

		panelSceneObjects();
		panelAssets(inputState);

		m_scene->Tick(deltaTime, m_GLDevice.get(), inputState);
	}

	void WindowsEditor::Render(int32 width, int32 height)
	{
		m_GLFrameBuffer->Activate();
		m_scene->Render(m_GLDevice.get(), m_GLFrameBuffer->GetWidth(), m_GLFrameBuffer->GetHeight(), m_LightmapEncodingInRGBM);
		m_GLFrameBuffer->Inactivate();
		
		m_guiWrapper->Render(width, height);
	}
	
	void WindowsEditor::Bake()
	{
		if (!m_baking)
			return;
		
		if (RemainingPrimitives.empty())
		{
			m_frameCount = 0;
		}

		//	TODO:	这里需要"聚合"的封装,目前只实现对单个对象的bake.
		Object* BeingBakingObject = m_scene->GetBeingBakingObject();
		
		if (m_frameCount == 0)
		{
			m_LightmapEncodingInRGBM = False;
			BeingBakingObject->glRenderableUnit->material.lock()->IsBeingBaking = True;
			//	XXX:	BeingBakingObject->BeforeBaking() must be called before Light::BeforeBaking(),
			BeingBakingObject->BeforeBaking();
			BeingBakingObject->glRenderableUnit->ComputeFormFactorMaterial.lock()->albedoTexture = BeingBakingObject->glRenderableUnit->material.lock()->albedoTexture;
			BeingBakingObject->glRenderableUnit->ComputeFormFactorMaterial.lock()->IDCumeMap = m_primitiveIDCubeMap;
			BeingBakingObject->glRenderableUnit->ComputeFormFactorMaterial.lock()->VisibilityTexture = m_GLVisibilityTexture;

			int32 RadiosityTextureWidth = BeingBakingObject->glRenderableUnit->staticMesh->GetRadiosityTextureWidth();
			int32 RadiosityTextureHeight =BeingBakingObject->glRenderableUnit->staticMesh->GetRadiosityTextureHeight();
			
			float* BlackData = new float[RadiosityTextureWidth * RadiosityTextureHeight * 4];
			for (int32 i = 0; i < RadiosityTextureWidth * RadiosityTextureHeight; ++i)
			{
				BlackData[i * 4 + 0] = 0;
				BlackData[i * 4 + 1] = 0;
				BlackData[i * 4 + 2] = 0;
				BlackData[i * 4 + 3] = 0;
			}
			
			m_RadiosityImage0 = std::make_shared<GLImageTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Bilinear, GLImageUnit_0, 0, GLImageAccess_ReadWrite);
			m_RadiosityImage0->LoadImage(
			RadiosityTextureWidth,
			RadiosityTextureHeight,
			BlackData);
			BeingBakingObject->glRenderableUnit->ComputeFormFactorMaterial.lock()->RadiosityImage0 = m_RadiosityImage0;
			
			m_RadiosityImage1 = std::make_shared<GLImageTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Bilinear, GLImageUnit_1, 0, GLImageAccess_ReadWrite);
			m_RadiosityImage1->LoadImage(
			RadiosityTextureWidth,
			RadiosityTextureHeight,
			BlackData);
			BeingBakingObject->glRenderableUnit->ComputeFormFactorMaterial.lock()->RadiosityImage1 = m_RadiosityImage1;
			
			m_ResidualImage0 = std::make_shared<GLImageTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point, GLImageUnit_2, 0, GLImageAccess_ReadWrite);
			m_ResidualImage0->LoadImage(
			RadiosityTextureWidth,
			RadiosityTextureHeight,
			BlackData);
			BeingBakingObject->glRenderableUnit->ComputeFormFactorMaterial.lock()->ResidualImage0 = m_ResidualImage0;

			m_ResidualImage1 = std::make_shared<GLImageTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point, GLImageUnit_3, 0, GLImageAccess_ReadWrite);
			m_ResidualImage1->LoadImage(
			RadiosityTextureWidth,
			RadiosityTextureHeight,
			BlackData);
			BeingBakingObject->glRenderableUnit->ComputeFormFactorMaterial.lock()->ResidualImage1 = m_ResidualImage1;

			delete[] BlackData;

			m_RadiosityTexture = std::make_unique<GLTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
			m_RadiosityTexture->LoadImage(
			RadiosityTextureWidth,
			RadiosityTextureHeight,
			Null);
			
			m_reconstructionPassFrameBuffer = std::make_unique<GLFrameBuffer>();
			m_reconstructionPassFrameBuffer->Resize(RadiosityTextureWidth, RadiosityTextureHeight);
			m_reconstructionPassFrameBuffer->AttachColor(GLAttachIndexColor0, m_RadiosityTexture->GetTarget(), m_RadiosityTexture.get());

			m_pRadiosityImageRawData = new float[RadiosityTextureWidth * RadiosityTextureHeight * sizeof(float) * 4];
			memset(m_pRadiosityImageRawData, 0, RadiosityTextureWidth * RadiosityTextureHeight * sizeof(float) * 4);
			
			m_pResidualImageRawData = new float[RadiosityTextureWidth * RadiosityTextureHeight * sizeof(float) * 4];
			memset(m_pResidualImageRawData, 0, RadiosityTextureWidth * RadiosityTextureHeight * sizeof(float) * 4);

			for (int32 i = 0; i < m_scene->GetLightCount(); ++i)
			{
				Object* AreaLight = m_scene->GetAreaLight(i);
				AreaLight->glRenderableUnit->staticMesh = CreateAreaLightMesh(AreaLight->XLength, AreaLight->YLength, AreaLight->LightPrecision);
				AreaLight->scale.x = 1.0f;
				AreaLight->scale.y = 1.0f;
				AreaLight->UpdateTransformMatrix();
				AreaLight->BeforeBaking();
				
				for (map<int32, Primitive>::iterator iter = AreaLight->glRenderableUnit->staticMesh->PrimitiveMap.begin();
						iter != AreaLight->glRenderableUnit->staticMesh->PrimitiveMap.end();
						++iter)
				{
					iter->second.Energy.x = AreaLight->Energy[0];
					iter->second.Energy.y = AreaLight->Energy[1];
					iter->second.Energy.z = AreaLight->Energy[2];
					iter->second.Energy.w = 1.0f;
					
					RemainingPrimitives.push(iter->second);
				}
			}
			
			m_GLPositionAttach = std::make_unique<GLTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
			m_GLPositionAttach->LoadImage(
			RadiosityTextureWidth,
			RadiosityTextureHeight,
			Null);

			m_GLNormalAttach = std::make_unique<GLTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
			m_GLNormalAttach->LoadImage(
			RadiosityTextureWidth,
			RadiosityTextureHeight,
			Null);
			
			m_GLGBufferFrameBuffer = std::make_unique<GLFrameBuffer>();
			m_GLGBufferFrameBuffer->Resize(RadiosityTextureWidth, RadiosityTextureHeight);
			m_GLGBufferFrameBuffer->AttachColor(GLAttachIndexColor0, m_GLPositionAttach->GetTarget(), m_GLPositionAttach.get());
			m_GLGBufferFrameBuffer->AttachColor(GLAttachIndexColor1, m_GLNormalAttach->GetTarget(), m_GLNormalAttach.get());
			
			m_GLGBufferFrameBuffer->Activate();
			{
				m_GLDevice->BeginBasePass(RadiosityTextureWidth, RadiosityTextureHeight);
				BeingBakingObject->DrawGBuffer(m_GLDevice.get());
			}
			m_GLGBufferFrameBuffer->Inactivate();

			float* m_pPositionRawData = new float[RadiosityTextureWidth * RadiosityTextureHeight * 4];
			float* m_pNormalRawData = new float[RadiosityTextureWidth * RadiosityTextureHeight * 4];
			
			m_GLPositionAttach->Fetch(m_pPositionRawData);
			m_GLNormalAttach->Fetch(m_pNormalRawData);

			m_RLBakingObjectPosition->LoadImage(RadiosityTextureWidth, RadiosityTextureHeight, m_pPositionRawData);
			m_RLBakingObjectNormal->LoadImage(RadiosityTextureWidth, RadiosityTextureHeight, m_pNormalRawData);
		
			delete[] m_pPositionRawData;
			delete[] m_pNormalRawData;

			m_RLBakeColorAttach->LoadImage(RadiosityTextureWidth, RadiosityTextureHeight, Null);
			m_RLBakeFrameBuffer->AttachColor(RLAttachIndexColor0, m_RLBakeColorAttach.get());

			m_RLBakePackingBuffer->AllocateMemorySpace(
						RadiosityTextureWidth * RadiosityTextureHeight * m_RLBakeColorAttach->GetDataSizePerPixel(),
						RLBufferUsage_StaticDraw);

			m_GLVisibilityTexture->LoadImage(RadiosityTextureWidth, RadiosityTextureHeight, Null);
		}
		
		if (!RemainingPrimitives.empty())
		{
			Primitive ShootingPrimitive = RemainingPrimitives.front();
			RemainingPrimitives.pop();
			
			std::unique_ptr<Object> PrimitiveObject = CreateObject(ShootingPrimitive);
			
			//	RL Visibility
			{
				int32 RadiosityTextureWidth = BeingBakingObject->glRenderableUnit->staticMesh->GetRadiosityTextureWidth();
				int32 RadiosityTextureHeight = BeingBakingObject->glRenderableUnit->staticMesh->GetRadiosityTextureHeight();
				
				m_rlShootingPrimitiveBuffer->Activate();
				RLShootingPrimitive* RLPrimitiveData = (RLShootingPrimitive*)m_rlShootingPrimitiveBuffer->Map(RLBufferAccessFlag_ReadWrite);
				RLPrimitiveData->Positions[0] = ShootingPrimitive.CentroidPosition;
				RLPrimitiveData->Positions[1] = ShootingPrimitive.Positions[0];
				RLPrimitiveData->Positions[2] = ShootingPrimitive.Positions[1];
				RLPrimitiveData->Positions[3] = ShootingPrimitive.Positions[2];
				m_rlShootingPrimitiveBuffer->Unmap();
				m_rlShootingPrimitiveBuffer->Inactivate();
				
				m_RLDevice->BeginBake(m_RLBakingObjectPosition.get(), m_RLBakingObjectNormal.get(), m_RLHammersleyTexture.get(), m_rlShootingPrimitiveBuffer.get());
				m_RLBakeFrameBuffer->Activate();
				m_RLDevice->SetViewport(0, 0, RadiosityTextureWidth, RadiosityTextureHeight);
				m_RLDevice->Clear();
				BeingBakingObject->UpdateRLMatrix(m_RLDevice.get());
				m_RLDevice->ExecuteCommands();
				m_RLBakeFrameBuffer->Inactivate();

				m_RLBakePackingBuffer->Activate();
				m_RLBakeColorAttach->Activate();
				m_RLBakeColorAttach->GetTexImage();
				m_RLBakeColorAttach->Inactivate();
				m_RLBakePackingBuffer->Inactivate();

				m_RLBakePackingBuffer->Activate();
				float * pPixels = static_cast<float *>(m_RLBakePackingBuffer->Map(RLBufferAccessFlag_ReadOnly));
				m_GLVisibilityTexture->LoadImage(RadiosityTextureWidth, RadiosityTextureHeight, pPixels);
				m_RLBakePackingBuffer->Unmap();
				m_RLBakePackingBuffer->Inactivate();
			}

			goto across;
			
			//	Visibility Pass
			{
				Camera Camera;
				Camera.zNear = 1.0f;
				Camera.zFar = 1000.0f;
				Camera.ascept = 1.0f;
				Camera.ascept *= static_cast<float>(PrimitiveIDTextureWidth);
				Camera.ascept /= static_cast<float>(PrimitiveIDTextureHeight);
				Camera.fovY = 90.0f * Deg2Rad;
				Camera.position = ShootingPrimitive.CentroidPosition;
				Camera.UpdatePerspectiveProjectionMatrix();
				
				//	FIXME:	这里需要定义渲染器里世界坐标系.
				
				//	+X
				m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor0, GLTextureTarget_CubeMap_Positive_X, m_primitiveIDCubeMap.get());
				//m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor1, GLTextureTarget_CubeMap_Positive_X, m_primitiveAlbedoCubeMap.get());
				m_visibilityPassFrameBuffer->Activate();
				{
					m_GLDevice->BeginVisibisityPass(PrimitiveIDTextureWidth, PrimitiveIDTextureHeight);
					Camera.lookAtDir = Right;
					Camera.UpdateViewMatrixRHUp(-Up);
					Camera.UpdateViewPerspectiveProjectionMatrix();
					CubeMatrices.ViewProjection_Positive_X = *Camera.GetViewPerspcetiveProjectionMatrix();
					Camera.UpdataGLParam(m_GLDevice.get());
					BeingBakingObject->DrawID(m_GLDevice.get());
				}
				m_visibilityPassFrameBuffer->Inactivate();
				m_visibilityPassFrameBuffer->ClearAttaches();
				
				//	-X
				m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor0, GLTextureTarget_CubeMap_Negative_X, m_primitiveIDCubeMap.get());
				m_visibilityPassFrameBuffer->Activate();
				{
					m_GLDevice->BeginVisibisityPass(PrimitiveIDTextureWidth, PrimitiveIDTextureHeight);
					Camera.lookAtDir = -Right;
					Camera.UpdateViewMatrixRHUp(-Up);
					Camera.UpdateViewPerspectiveProjectionMatrix();
					CubeMatrices.ViewProjection_Negative_X = *Camera.GetViewPerspcetiveProjectionMatrix();
					Camera.UpdataGLParam(m_GLDevice.get());
					BeingBakingObject->DrawID(m_GLDevice.get());
					}
				m_visibilityPassFrameBuffer->Inactivate();
				m_visibilityPassFrameBuffer->ClearAttaches();
				
				//	+Y
				m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor0, GLTextureTarget_CubeMap_Positive_Y, m_primitiveIDCubeMap.get());
				m_visibilityPassFrameBuffer->Activate();
				{
					m_GLDevice->BeginVisibisityPass(PrimitiveIDTextureWidth, PrimitiveIDTextureHeight);
					Camera.lookAtDir = Up;
					Camera.UpdateViewMatrixRHUp(Forward);
					Camera.UpdateViewPerspectiveProjectionMatrix();
					CubeMatrices.ViewProjection_Positive_Y = *Camera.GetViewPerspcetiveProjectionMatrix();
					Camera.UpdataGLParam(m_GLDevice.get());
					BeingBakingObject->DrawID(m_GLDevice.get());
				}
				m_visibilityPassFrameBuffer->Inactivate();
				m_visibilityPassFrameBuffer->ClearAttaches();
				
				//	-Y
				m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor0, GLTextureTarget_CubeMap_Negative_Y, m_primitiveIDCubeMap.get());
				m_visibilityPassFrameBuffer->Activate();
				{
					m_GLDevice->BeginVisibisityPass(PrimitiveIDTextureWidth, PrimitiveIDTextureHeight);
					Camera.lookAtDir = -Up;
					Camera.UpdateViewMatrixRHUp(-Forward);
					Camera.UpdateViewPerspectiveProjectionMatrix();
					CubeMatrices.ViewProjection_Negative_Y = *Camera.GetViewPerspcetiveProjectionMatrix();
					Camera.UpdataGLParam(m_GLDevice.get());
					BeingBakingObject->DrawID(m_GLDevice.get());
				}
				m_visibilityPassFrameBuffer->Inactivate();
				m_visibilityPassFrameBuffer->ClearAttaches();
				
				//	+Z
				m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor0, GLTextureTarget_CubeMap_Positive_Z, m_primitiveIDCubeMap.get());
				m_visibilityPassFrameBuffer->Activate();
				{
					m_GLDevice->BeginVisibisityPass(PrimitiveIDTextureWidth, PrimitiveIDTextureHeight);
					Camera.lookAtDir = Forward;
					Camera.UpdateViewMatrixRHUp(-Up);
					Camera.UpdateViewPerspectiveProjectionMatrix();
					CubeMatrices.ViewProjection_Positive_Z = *Camera.GetViewPerspcetiveProjectionMatrix();
					Camera.UpdataGLParam(m_GLDevice.get());
					BeingBakingObject->DrawID(m_GLDevice.get());
				}
				m_visibilityPassFrameBuffer->Inactivate();
				m_visibilityPassFrameBuffer->ClearAttaches();
				
				//	-Z
				m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor0, GLTextureTarget_CubeMap_Negative_Z, m_primitiveIDCubeMap.get());
				m_visibilityPassFrameBuffer->Activate();
				{
					m_GLDevice->BeginVisibisityPass(PrimitiveIDTextureWidth, PrimitiveIDTextureHeight);
					Camera.lookAtDir = -Forward;
					Camera.UpdateViewMatrixRHUp(-Up);
					Camera.UpdateViewPerspectiveProjectionMatrix();
					CubeMatrices.ViewProjection_Negative_Z = *Camera.GetViewPerspcetiveProjectionMatrix();
					Camera.UpdataGLParam(m_GLDevice.get());
					BeingBakingObject->DrawID(m_GLDevice.get());
				}
				m_visibilityPassFrameBuffer->Inactivate();
				m_visibilityPassFrameBuffer->ClearAttaches();
			}

			across:

			//	Reconstruction Pass
			m_reconstructionPassFrameBuffer->Activate();
			{
				int32 RadiosityTextureWidth = BeingBakingObject->glRenderableUnit->staticMesh->GetRadiosityTextureWidth();
				int32 RadiosityTextureHeight = BeingBakingObject->glRenderableUnit->staticMesh->GetRadiosityTextureHeight();
				m_GLDevice->BeginReconstrucionPass(RadiosityTextureWidth, RadiosityTextureHeight);

				//	Each camera can have two usages for shaders, perspective camera or ortho camera according to
				//	use viewPerspectiveProjectionMatrix or viewOrthoProjectionMatrix to transform vertex.
				//	For current usage, perspective transformation is used for transform vertex back to
				//	visibility testing camera's perspective view to do depth testing, and
				//	ortho camera is used to rasterize each primitive on top of it to perfom baking.
				//	We update camera for perspective usage and ortho usage respectively, and so independently.
				Camera Camera;
				Camera.frameCount = m_frameCount;
				////	Used for constructing perspective projection matrix for doing depth test to cubemap,
				////	same parameters with building cube map camera.
				Camera.position = ShootingPrimitive.CentroidPosition;
				Camera.lookAtDir = ShootingPrimitive.Normal;
				Camera.UpdateViewMatrixRH();
				Camera.zNear = 1.0f;
				Camera.zFar = 1000.0f;
				Camera.ascept = 1.0f;
				Camera.ascept *= static_cast<float>(PrimitiveIDTextureWidth);
				Camera.ascept /= static_cast<float>(PrimitiveIDTextureHeight);
				Camera.fovY = 90.0f * Deg2Rad;
				Camera.UpdatePerspectiveProjectionMatrix();
				Camera.UpdateViewPerspectiveProjectionMatrix();
				
				ShooterInfo ShooterInfo;
				ShooterInfo.Position = ShootingPrimitive.CentroidPosition;
				ShooterInfo.Normal = Vector4(ShootingPrimitive.Normal.x, ShootingPrimitive.Normal.y, ShootingPrimitive.Normal.z, 0);
				ShooterInfo.Energy = ShootingPrimitive.Energy;
				ShooterInfo.SurfaceArea = Vector4(ShootingPrimitive.SurfaceArea, ShootingPrimitive.SurfaceArea, ShootingPrimitive.SurfaceArea, ShootingPrimitive.SurfaceArea);
				m_GLDevice->UploadGlobalShaderData(GLShaderDataAlias_ShooterInfo, sizeof(ShooterInfo), &ShooterInfo);
				m_GLDevice->UploadGlobalShaderData(GLShaderDataAlias_CubeMatrices, sizeof(CubeMatrices), &CubeMatrices);

				BeingBakingObject->BeforeComputeFormFactor(m_GLDevice.get());
				for (int32 PrimitiveIndex = 0; PrimitiveIndex < BeingBakingObject->glRenderableUnit->staticMesh->indexCount / 3; ++PrimitiveIndex)
				{
					Primitive BakingPrimitive = BeingBakingObject->glRenderableUnit->staticMesh->PrimitiveMap[PrimitiveIndex];
					
					Camera.position = Vector3(BakingPrimitive.UV1CentroidPosition.x, BakingPrimitive.UV1CentroidPosition.y, BakingPrimitive.UV1CentroidPosition.z);
					Camera.position += BakingPrimitive.UV1Normal * Vector3(1.0, 1.0, 1.0);
					Camera.lookAtDir = -BakingPrimitive.UV1Normal;
					Camera.UpdateViewMatrixRH();
					float Left;
					float Right;
					float Bottom;
					float Top;
					float ZNear;
					float ZFar;
					BeingBakingObject->glRenderableUnit->staticMesh->CalculateOrthoParameters(
						PrimitiveIndex,
						1,
						*Camera.GetViewMatrix(),
						Left,
						Right,
						Bottom,
						Top,
						ZNear,
						ZFar);
					Camera.OrthoParams.Left = Left;
					Camera.OrthoParams.Right = Right;
					Camera.OrthoParams.Bottom = Bottom;
					Camera.OrthoParams.Top = Top;
					Camera.OrthoParams.ZNear = 0;
					Camera.OrthoParams.ZFar = max(abs(ZNear), abs(ZFar));
					Camera.UpdateOrthoProjctionMatrix();
					Camera.UpdateViewOrthoProjctionMatrix();
					Camera.UpdataGLParam(m_GLDevice.get());

					BeingBakingObject->DrawPrimitive(m_GLDevice.get(), PrimitiveIndex, 1);
				}
				BeingBakingObject->AfterComputeFormFactor();
			}
			m_reconstructionPassFrameBuffer->Inactivate();

			if (m_frameCount % 2 == 0)
			{
				BeingBakingObject->glRenderableUnit->material.lock()->lightmapImageTexture = m_RadiosityImage1;
			}
			else
			{
				BeingBakingObject->glRenderableUnit->material.lock()->lightmapImageTexture = m_RadiosityImage0;
			}
			
			if (RemainingPrimitives.empty())
			{
				//	Pick Pass, pick next emmiter
				//	TODO:	pbo
				if (m_frameCount % 2 == 0)
				{
					m_ResidualImage1->Fetch(m_pResidualImageRawData);
				}
				else
				{
					m_ResidualImage0->Fetch(m_pResidualImageRawData);
				}
				
				int32 RadiosityTextureWidth = BeingBakingObject->glRenderableUnit->staticMesh->GetRadiosityTextureWidth();
				int32 RadiosityTextureHeight = BeingBakingObject->glRenderableUnit->staticMesh->GetRadiosityTextureHeight();

				std::map<int32, float> PrimitiveYMap;
				std::map<int32, Vector3> PrimitiveRGBMap;
				
				//	pick
				int32 MaxYPrimitiveID = -1;
				float MaxY = 0;
				for (int32 i = 0; i < RadiosityTextureHeight; ++i)
				{
					for (int32 j = 0; j < RadiosityTextureWidth; ++j)
					{
						float R = m_pResidualImageRawData[(i * RadiosityTextureWidth + j) * 4 + 0];
						float G = m_pResidualImageRawData[(i * RadiosityTextureWidth + j) * 4 + 1];
						float B = m_pResidualImageRawData[(i * RadiosityTextureWidth + j) * 4 + 2];
						
						float Y = R * 0.30f + G * 0.59f + B * 0.11f;

						int32 PrimitiveID = static_cast<int32>(m_pResidualImageRawData[(i * RadiosityTextureWidth + j) * 4 + 3]);

						if (PrimitiveYMap.find(PrimitiveID) == PrimitiveYMap.end())
						{
							PrimitiveYMap[PrimitiveID] = Y;
							PrimitiveRGBMap[PrimitiveID] = Vector3(R, G, B);
						}
						else
						{
							PrimitiveYMap[PrimitiveID] += Y;
							PrimitiveRGBMap[PrimitiveID] += Vector3(R, G, B);
						}

						if (PrimitiveYMap[PrimitiveID] > MaxY)
						{
							MaxY = PrimitiveYMap[PrimitiveID];
							MaxYPrimitiveID = PrimitiveID;
						}
					}
				}
				
				Vector3 RGB = PrimitiveRGBMap[MaxYPrimitiveID];
				float ScaledUV1Area = BeingBakingObject->glRenderableUnit->staticMesh->PrimitiveMap[MaxYPrimitiveID].ScaledUV1Area;
				RGB.x /= ScaledUV1Area;
				RGB.y /= ScaledUV1Area;
				RGB.z /= ScaledUV1Area;
				m_currentMaxY = RGB.r * 0.30f + RGB.g * 0.59f + RGB.b * 0.11f;

				if (m_currentMaxY < m_thresholdY)
				{
					m_baking = False;
					RestoreLightMesh();
					
					return ;
				}
				
				BeingBakingObject->glRenderableUnit->staticMesh->PrimitiveMap[MaxYPrimitiveID].Energy = Vector4(RGB.x, RGB.y, RGB.z, 1.0);
				RemainingPrimitives.push(BeingBakingObject->glRenderableUnit->staticMesh->PrimitiveMap[MaxYPrimitiveID]);

				for (int32 i = 0; i < RadiosityTextureHeight; ++i)
				{
					for (int32 j = 0; j < RadiosityTextureWidth; ++j)
					{
						int32 PrimitiveID = static_cast<int32>(m_pResidualImageRawData[(i * RadiosityTextureWidth + j) * 4 + 3]);

						if (PrimitiveID == MaxYPrimitiveID)
						{
							m_pResidualImageRawData[(i * RadiosityTextureWidth + j) * 4 + 0] = 0;
							m_pResidualImageRawData[(i * RadiosityTextureWidth + j) * 4 + 1] = 0;
							m_pResidualImageRawData[(i * RadiosityTextureWidth + j) * 4 + 2] = 0;
						}
					}
				}
				
				if (m_frameCount % 2 == 0)
				{
					m_ResidualImage1->LoadImage(RadiosityTextureWidth, RadiosityTextureHeight, m_pResidualImageRawData);
				}
				else
				{
					m_ResidualImage0->LoadImage(RadiosityTextureWidth, RadiosityTextureHeight, m_pResidualImageRawData);
				}
			}
		}
		
		++m_frameCount;
	}

	WindowsEditor::~WindowsEditor()
	{
		if (m_pRadiosityImageRawData)
		{
			delete[] m_pRadiosityImageRawData;
		}

		if (m_pResidualImageRawData)
		{
			delete[] m_pResidualImageRawData;
		}
	}
}