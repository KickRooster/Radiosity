#include "WindowsEditor.h"
#include <SDL/SDL_filesystem.h>
#include "../../../Helper/TextureOperator.h"
#include "../3rdParty/LinearLeastSquaresStitch/Sitich.h"
#include "../../../Helper/Helper.h"
#include <queue>


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

		m_arealLightMaterial->glVertexShader = m_assetManager->glVertexShaderMap["default"];
		m_arealLightMaterial->glVertexShader.lock()->Attach(m_arealLightMaterial.get());
		m_arealLightMaterial->glFragmentShader = m_assetManager->glFragmentShaderMap["default"];
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
		
		m_ViewCubeMapMaterial = std::make_shared<Material>();
		m_ViewCubeMapMaterial->glVertexShader = m_assetManager->glVertexShaderMap["viewCubeMap"];
		m_ViewCubeMapMaterial->glVertexShader.lock()->Attach(m_ViewCubeMapMaterial.get());
		m_ViewCubeMapMaterial->glFragmentShader = m_assetManager->glFragmentShaderMap["viewCubeMap"];
		m_ViewCubeMapMaterial->glFragmentShader.lock()->Attach(m_ViewCubeMapMaterial.get());
		
		//////////////////////////////////////////////////////////////////////////
		
		m_PickShooterMaterial = std::make_shared<Material>();
		m_PickShooterMaterial->glVertexShader = m_assetManager->glVertexShaderMap["pickShooter"];
		m_PickShooterMaterial->glVertexShader.lock()->Attach(m_PickShooterMaterial.get());
		m_PickShooterMaterial->glFragmentShader = m_assetManager->glFragmentShaderMap["pickShooter"];
		m_PickShooterMaterial->glFragmentShader.lock()->Attach(m_PickShooterMaterial.get());
		
		//////////////////////////////////////////////////////////////////////////
		
		m_areaLightMesh = std::make_shared<StaticMesh>();

		m_areaLightMesh->vertexCount = 6;
		m_areaLightMesh->pPositions = new Vector4[m_areaLightMesh->vertexCount];
		m_areaLightMesh->pNormals = new Vector3[m_areaLightMesh->vertexCount];
		m_areaLightMesh->pUV0s = new Vector2[m_areaLightMesh->vertexCount];
		m_areaLightMesh->pUV1s = new Vector2[m_areaLightMesh->vertexCount];
		
		float lightScale = 10.0f;

		m_areaLightMesh->pPositions[0].x = -0.5f * lightScale;
		m_areaLightMesh->pPositions[0].y = -0.5f * lightScale ;
		m_areaLightMesh->pPositions[0].z = 0;
		m_areaLightMesh->pPositions[0].w = 1.0f;
		m_areaLightMesh->pNormals[0].x = 0;
		m_areaLightMesh->pNormals[0].y = 0;
		m_areaLightMesh->pNormals[0].z = 1.0f;
		m_areaLightMesh->pUV0s[0] = Vector2(0, 0);
		m_areaLightMesh->pUV1s[0] = Vector2(0, 0);
		
		m_areaLightMesh->pPositions[1].x = 0.5f * lightScale;
		m_areaLightMesh->pPositions[1].y = -0.5f * lightScale ;
		m_areaLightMesh->pPositions[1].z = 0;
		m_areaLightMesh->pPositions[1].w = 1.0f;
		m_areaLightMesh->pNormals[1].x = 0;
		m_areaLightMesh->pNormals[1].y = 0;
		m_areaLightMesh->pNormals[1].z = 1.0f;
		m_areaLightMesh->pUV0s[1] = Vector2(1.0, 0);
		m_areaLightMesh->pUV1s[1] = Vector2(1.0, 0);

		m_areaLightMesh->pPositions[2].x = 0.5f * lightScale;
		m_areaLightMesh->pPositions[2].y = 0.5f * lightScale ;
		m_areaLightMesh->pPositions[2].z = 0;
		m_areaLightMesh->pPositions[2].w = 1.0f;
		m_areaLightMesh->pNormals[2].x = 0;
		m_areaLightMesh->pNormals[2].y = 0;
		m_areaLightMesh->pNormals[2].z = 1.0f;
		m_areaLightMesh->pUV0s[2] = Vector2(1.0f, 1.0f);
		m_areaLightMesh->pUV1s[2] = Vector2(1.0f, 1.0f);

		m_areaLightMesh->pPositions[3].x = 0.5f * lightScale;
		m_areaLightMesh->pPositions[3].y = 0.5f * lightScale ;
		m_areaLightMesh->pPositions[3].z = 0;
		m_areaLightMesh->pPositions[3].w = 1.0f;
		m_areaLightMesh->pNormals[3].x = 0;
		m_areaLightMesh->pNormals[3].y = 0;
		m_areaLightMesh->pNormals[3].z = 1.0f;
		m_areaLightMesh->pUV0s[3] = Vector2(1.0f, 1.0f);
		m_areaLightMesh->pUV1s[3] = Vector2(1.0f, 1.0f);

		m_areaLightMesh->pPositions[4].x = -0.5f * lightScale;
		m_areaLightMesh->pPositions[4].y = 0.5f * lightScale ;
		m_areaLightMesh->pPositions[4].z = 0;
		m_areaLightMesh->pPositions[4].w = 1.0f;
		m_areaLightMesh->pNormals[4].x = 0;
		m_areaLightMesh->pNormals[4].y = 0;
		m_areaLightMesh->pNormals[4].z = 1.0f;
		m_areaLightMesh->pUV0s[4] = Vector2(0, 1.0f);
		m_areaLightMesh->pUV1s[4] = Vector2(0, 1.0f);

		m_areaLightMesh->pPositions[5].x = -0.5f * lightScale;
		m_areaLightMesh->pPositions[5].y = -0.5f * lightScale ;
		m_areaLightMesh->pPositions[5].z = 0;
		m_areaLightMesh->pPositions[5].w = 1.0f;
		m_areaLightMesh->pNormals[5].x = 0;
		m_areaLightMesh->pNormals[5].y = 0;
		m_areaLightMesh->pNormals[5].z = 1.0f;
		m_areaLightMesh->pUV0s[5] = Vector2(0, 0);
		m_areaLightMesh->pUV1s[5] = Vector2(0, 0);

		m_areaLightMesh->indexCount = 6;
		m_areaLightMesh->pIndices = new uint32[m_areaLightMesh->indexCount];
		
		m_areaLightMesh->pIndices[0] = 0;
		m_areaLightMesh->pIndices[1] = 1;
		m_areaLightMesh->pIndices[2] = 2;
		m_areaLightMesh->pIndices[3] = 3;
		m_areaLightMesh->pIndices[4] = 4;
		m_areaLightMesh->pIndices[5] = 5;
		
		//////////////////////////////////////////////////////////////////////////
		m_postprocessMesh = std::make_shared<StaticMesh>();
		m_postprocessMesh->pPositions = new Vector4[3];
		m_postprocessMesh->pUV0s = new Vector2[3];
		m_postprocessMesh->vertexCount = 3;

		m_postprocessMesh->pPositions[0].x = -1.0f;
		m_postprocessMesh->pPositions[0].y = -1.0f;
		m_postprocessMesh->pPositions[0].z = 0.9f;
		m_postprocessMesh->pPositions[0].w = 1.0f;
		m_postprocessMesh->pUV0s[0].x = 0;
		m_postprocessMesh->pUV0s[0].y = 1.0f;

		m_postprocessMesh->pPositions[1].x = 3.0f;
		m_postprocessMesh->pPositions[1].y = -1.0f;
		m_postprocessMesh->pPositions[1].z = 0.9f;
		m_postprocessMesh->pPositions[1].w = 1.0f;
		m_postprocessMesh->pUV0s[1].x = 2.0f;
		m_postprocessMesh->pUV0s[1].y = 1.0f;

		m_postprocessMesh->pPositions[2].x = -1.0f;
		m_postprocessMesh->pPositions[2].y = 3.0f;
		m_postprocessMesh->pPositions[2].z = 0.9f;
		m_postprocessMesh->pPositions[2].w = 1.0f;
		m_postprocessMesh->pUV0s[2].x = 0;
		m_postprocessMesh->pUV0s[2].y = -1.0f;

		m_postprocessMesh->pIndices = new uint32[3];
		m_postprocessMesh->indexCount = 3;
		m_postprocessMesh->pIndices[0] = 0;
		m_postprocessMesh->pIndices[1] = 1;
		m_postprocessMesh->pIndices[2] = 2;
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
		ImGui::SetNextWindowSize(ImVec2(600, 400));

		ImGui::Begin("Assets");
		{
			inputState.inlvaid = False;
			//ImGui::TextColored(ImVec4(0, 0.6f, 1.0f, 1.0f), "Create: ");

			Bool pressedOK = False;
			string fileName;

			if (ImGui::Button("Create Scene"))
				ImGui::OpenPopup("Scene");

			popupNamingTick("Scene", m_assetManager->sceneExt, inputState, pressedOK, fileName);

			if (pressedOK)
			{
				m_assetManager->CreateScene(fileName.c_str());
				pressedOK = False;
			}

			ImGui::SameLine();

			if (ImGui::Button("Create Prefab"))
				ImGui::OpenPopup("Prefab");

			popupNamingTick("Prefab", m_assetManager->prefabExt, inputState, pressedOK, fileName);

			if (pressedOK)
			{
				m_assetManager->CreatePrefab(fileName.c_str());
				pressedOK = False;
			}

			ImGui::SameLine();

			if (ImGui::Button("Create Material"))
				ImGui::OpenPopup("Material");

			popupNamingTick("Material", m_assetManager->materialExt, inputState, pressedOK, fileName);

			if (pressedOK)
			{
				m_assetManager->CreateMaterial(fileName.c_str());
				pressedOK = False;
			}

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

			if (ImGui::Selectable("Scene", &categorySelections[AssetType_Scene]))
			{
				selectedCategory = AssetType_Scene;
				categoryName = "Scene";
			}

			if (ImGui::Selectable("Prefab", &categorySelections[AssetType_Prefab]))
			{
				selectedCategory = AssetType_Prefab;
				categoryName = "Prefab";
			}

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

			if (ImGui::Selectable("GLSL", &categorySelections[AssetType_GLShader]))
			{
				selectedCategory = AssetType_GLShader;
				categoryName = "GLSL";
			}
			
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

				if (ImGui::Button("Add to Scene"))
					m_scene->AddObject(createObject(selectedPrefab));

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
				if (ImGui::RadioButton("Normal Map", &radioIndex, 1))
				{
					ImGui::OpenPopup("Normal Map...");

					memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
					selectedFileIndexInInspector = InvalidIndex;
					selectedFileLastFrameIndexInInspector = InvalidIndex;
				}

				//	Metallic Map
				if (ImGui::RadioButton("Metallic Map", &radioIndex, 2))
				{
					ImGui::OpenPopup("Metallic Map...");

					memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
					selectedFileIndexInInspector = InvalidIndex;
					selectedFileLastFrameIndexInInspector = InvalidIndex;
				}

				//	Roughness Map
				if (ImGui::RadioButton("Roughness Map", &radioIndex, 3))
				{
					ImGui::OpenPopup("Roughness Map...");

					memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
					selectedFileIndexInInspector = InvalidIndex;
					selectedFileLastFrameIndexInInspector = InvalidIndex;
				}

				//	GL Vertex Shader
				if (ImGui::RadioButton("GL VS", &radioIndex, 5))
				{
					ImGui::OpenPopup("GL Vertex Shader...");

					memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
					selectedFileIndexInInspector = InvalidIndex;
					selectedFileLastFrameIndexInInspector = InvalidIndex;
					ImGui::SameLine();
				}

				//	GL Fragment Shader
				if (ImGui::RadioButton("GL FS", &radioIndex, 6))
				{
					ImGui::OpenPopup("GL Fragment Shader...");

					memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
					selectedFileIndexInInspector = InvalidIndex;
					selectedFileLastFrameIndexInInspector = InvalidIndex;
					ImGui::SameLine();
				}

				//	RL Vertex Shader
				if (ImGui::RadioButton("RL VS", &radioIndex, 7))
				{
					ImGui::OpenPopup("RL Vertex Shader...");

					memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
					selectedFileIndexInInspector = InvalidIndex;
					selectedFileLastFrameIndexInInspector = InvalidIndex;
					ImGui::SameLine();
				}

				//	RL Ray Shader
				if (ImGui::RadioButton("RL RS", &radioIndex, 8))
				{
					ImGui::OpenPopup("RL Ray Shader...");

					memset(fileSelectionsInInspector, false, sizeof(fileSelectionsInInspector[0]) * maxFileSize);
					selectedFileIndexInInspector = InvalidIndex;
					selectedFileLastFrameIndexInInspector = InvalidIndex;
					ImGui::SameLine();
				}

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
				popupFileSelectingTick(
					FileType_Texture,
					"Normal Map...",
					fileSelectionsInInspector,
					selectedFileIndexInInspector,
					selectedFileLastFrameIndexInInspector,
					inputState,
					pressedOK,
					fileName);

				if (pressedOK)
				{
					selectedMaterial.lock()->normalMapName = fileName;
					selectedMaterial.lock()->normalMap = m_assetManager->textureMap[fileName];
					selectedMaterial.lock()->normalMap.lock()->BeginUse();
					pressedOK = False;
				}

				if (!selectedMaterial.expired())
					ImGui::Button(selectedMaterial.lock()->normalMapName.c_str());

				//	Metallic Map
				popupFileSelectingTick(
					FileType_Texture,
					"Metallic Map...",
					fileSelectionsInInspector,
					selectedFileIndexInInspector,
					selectedFileLastFrameIndexInInspector,
					inputState,
					pressedOK,
					fileName);

				if (pressedOK)
				{
					selectedMaterial.lock()->metallicTextureName = fileName;
					selectedMaterial.lock()->metallicTexture = m_assetManager->textureMap[fileName];
					selectedMaterial.lock()->metallicTexture.lock()->BeginUse();
					pressedOK = False;
				}

				if (!selectedMaterial.expired())
					ImGui::Button(selectedMaterial.lock()->metallicTextureName.c_str());

				//	Roughness Map
				popupFileSelectingTick(
					FileType_Texture,
					"Roughness Map...",
					fileSelectionsInInspector,
					selectedFileIndexInInspector,
					selectedFileLastFrameIndexInInspector,
					inputState,
					pressedOK,
					fileName);

				if (pressedOK)
				{
					selectedMaterial.lock()->roughnessTextureName = fileName;
					selectedMaterial.lock()->roughnessTexture = m_assetManager->textureMap[fileName];
					selectedMaterial.lock()->roughnessTexture.lock()->BeginUse();
					pressedOK = False;
				}

				if (!selectedMaterial.expired())
					ImGui::Button(selectedMaterial.lock()->roughnessTextureName.c_str());
				
				//	GL Vertex Shader
				popupFileSelectingTick(
					FileType_GLSL_Vertex,
					"GL Vertex Shader...",
					fileSelectionsInInspector,
					selectedFileIndexInInspector,
					selectedFileLastFrameIndexInInspector,
					inputState,
					pressedOK,
					fileName);

				if (pressedOK)
				{
					selectedMaterial.lock()->glVertexShaderName = fileName;
					selectedMaterial.lock()->glVertexShader = m_assetManager->glVertexShaderMap[fileName];
					pressedOK = False;
				}

				if (!selectedMaterial.expired())
					ImGui::Button(selectedMaterial.lock()->glVertexShaderName.c_str());

				//	GLSL Fragment Shader
				popupFileSelectingTick(
					FileType_GLSL_Fragment,
					"GL Fragment Shader...",
					fileSelectionsInInspector,
					selectedFileIndexInInspector,
					selectedFileLastFrameIndexInInspector,
					inputState,
					pressedOK,
					fileName);

				if (pressedOK)
				{
					selectedMaterial.lock()->glFragmentShaderName = fileName;
					selectedMaterial.lock()->glFragmentShader = m_assetManager->glFragmentShaderMap[fileName];
					pressedOK = False;
				}

				if (!selectedMaterial.expired())
					ImGui::Button(selectedMaterial.lock()->glFragmentShaderName.c_str());

				ImGui::EndChild();
				ImGui::PopStyleVar();

				ImGui::BeginChild("Albedo Color", ImVec2(0, 50), true, 0);
				ImGui::ColorEdit4("Color", selectedMaterial.lock()->albedoColor);
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

	void EditTransform(const ImVec2 & regionTopLeft, const ImVec2 & regionSize, const float *cameraView, float *cameraProjection, float* matrix)
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
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

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

		ImGui::End();

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(regionTopLeft.x, regionTopLeft.y, regionSize.x, regionSize.y);
		ImGuizmo::SetDrawlist();
		ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL);
	}

	void WindowsEditor::guizmoTick(const ImVec2 & regionTopLeft, const ImVec2 & regionSize, void * pViewMatrix, void * pProjectionMatrix, Object * pSelectedObject)
	{
		ImGuizmo::BeginFrame(regionSize);

		ImGui::Begin("Matrix Inspector");

		EditTransform(regionTopLeft, regionSize, (float*)pViewMatrix, (float *)pProjectionMatrix, (float *)pSelectedObject->GetObject2WorldMatrix());

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

		material->IsOccluder = True;	//	TODO:	�����û��¶�ڱ༭����

		defaultObject->glRenderableUnit->material = material;
		defaultObject->glRenderableUnit->DrawGBufferMaterial = m_DrawGBufferMaterial;
		defaultObject->glRenderableUnit->DrawIDMaterial = m_DrawIDMaterial;
		defaultObject->glRenderableUnit->ComputeFormFactorMaterial = m_ComputeFormFactorMaterial;
		defaultObject->glRenderableUnit->ViewCubeMapMaterial = m_ViewCubeMapMaterial;
		defaultObject->glRenderableUnit->PickShooterMaterial = m_PickShooterMaterial;

		defaultObject->rlRenderableUnit = std::make_shared<RLRenderableUnit>();
		defaultObject->rlRenderableUnit->staticMesh = m_assetManager->staticMeshMap[prefab.lock()->staticMeshName];
		defaultObject->rlRenderableUnit->material = material;
		material->rlVertexShader.lock()->Attach(defaultObject->rlRenderableUnit.get());
		material->rlRayShader.lock()->Attach(defaultObject->rlRenderableUnit.get());
		
		defaultObject->Initialize(m_GLDevice.get(), False);

		return defaultObject;
	}

	std::shared_ptr<Core::Object> WindowsEditor::createObject(std::shared_ptr<Object> object)
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

		material->IsOccluder = True;	//	TODO:	�����û��¶�ڱ༭����

		object->glRenderableUnit->material = material;
		object->glRenderableUnit->DrawGBufferMaterial = m_DrawGBufferMaterial;
		object->glRenderableUnit->DrawIDMaterial = m_DrawIDMaterial;
		object->glRenderableUnit->ComputeFormFactorMaterial = m_ComputeFormFactorMaterial;
		object->glRenderableUnit->ViewCubeMapMaterial = m_ViewCubeMapMaterial;
		object->glRenderableUnit->PickShooterMaterial = m_PickShooterMaterial;

		object->rlRenderableUnit = std::make_shared<RLRenderableUnit>();
		object->rlRenderableUnit->staticMesh = m_assetManager->staticMeshMap[object->staticMeshName];
		object->rlRenderableUnit->material = material;
		material->rlVertexShader.lock()->Attach(object->rlRenderableUnit.get());
		material->rlRayShader.lock()->Attach(object->rlRenderableUnit.get());

		object->name = object->staticMeshName;

		object->Initialize(m_GLDevice.get(), True);

		return object;
	}

	std::shared_ptr<Object> WindowsEditor::createAreaLight()
	{
		std::shared_ptr<Object> areaLight = std::make_shared<Object>();
		ctd::string name = "Area Light";
		areaLight->name = name;

		areaLight->glRenderableUnit = std::make_unique<GLRenderableUnit>();
		areaLight->glRenderableUnit->staticMesh = m_areaLightMesh;
		areaLight->glRenderableUnit->material = m_arealLightMaterial;
		
		areaLight->position = Vector3(0, 2.0f, 25.0f);
		areaLight->eulerAngle = Vector3(180.0f, 0, 0);

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
		
		PrimitiveMaterial->rlVertexShader.lock()->Attach(PrimitiveObject->rlRenderableUnit.get());
		PrimitiveMaterial->rlRayShader.lock()->Attach(PrimitiveObject->rlRenderableUnit.get());

		PrimitiveMesh->BeginUse();
		PrimitiveObject->rlRenderableUnit->Commit();

		return PrimitiveObject;
	}
	
	void WindowsEditor::SaveLightmap(std::string Name, int32 Width, int32 Height)
	{
		if ((m_frameCount - 1) % 2 == 0)
		{
			m_RadiosityImage1->Fetch(m_pRadiosityImageRawData);
		}
		else
		{
			m_RadiosityImage0->Fetch(m_pRadiosityImageRawData);
		}

		uint8* LightmapRawData = new uint8[Width * Height * sizeof(uint8) * 4];

		for (int32 i = 0; i < Height; ++i)
		{
			for (int32 j = 0; j < Width; ++j)
			{
				float R = m_pRadiosityImageRawData[(i * Width + j) * 4 + 0];
				float G = m_pRadiosityImageRawData[(i * Width + j) * 4 + 1];
				float B = m_pRadiosityImageRawData[(i * Width + j) * 4 + 2];
				//float A = m_pRadiosityImageRawData[(i * Width + j) * 4 + 3];

				int32 IntR = static_cast<int32>(R * 255.0f);
				int32 IntG = static_cast<int32>(G * 255.0f);
				int32 IntB = static_cast<int32>(B * 255.0f);
				
				LightmapRawData[(i * Width + j) * 4 + 0] = Clamp(IntR, 0, 255);
				LightmapRawData[(i * Width + j) * 4 + 1] = Clamp(IntG, 0, 255);
				LightmapRawData[(i * Width + j) * 4 + 2] = Clamp(IntB, 0, 255);
				LightmapRawData[(i * Width + j) * 4 + 3] = 255;
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
		m_GLDebugViewFrameBuffer(std::make_unique<GLFrameBuffer>()),
		m_RLBakingObjectPosition(std::make_unique<RLTexture2D>(RLIinternalFormat_RGBA, RLPixelFormat_RGBA, RLDataType_Float, RLTextureWrapMode_Clamp, RLTextureFilterMode_Point)),
		m_RLBakingObjectNormal(std::make_unique<RLTexture2D>(RLIinternalFormat_RGBA, RLPixelFormat_RGBA, RLDataType_Float, RLTextureWrapMode_Clamp, RLTextureFilterMode_Point)),
		m_RLBakeFrameBuffer(std::make_unique<RLFrameBuffer>()),
		m_RLBakeColorAttach(std::make_unique<RLTexture2D>(RLIinternalFormat_RGBA, RLPixelFormat_RGBA, RLDataType_Float, RLTextureWrapMode_Clamp, RLTextureFilterMode_Point)),
		m_RLBakePackingBuffer(std::make_unique<RLBuffer>(RLBufferTarget_PixelPackBuffer)),
		m_GLVisibilityTexture(std::make_shared<GLTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point)),
		m_frameCount(0),
		m_baking(False),
		m_thresold(0.0001f)
	{
		//	Visibility Pass
		m_primitiveIDCubeMap = std::make_unique<GLTexture>(GLTextureTarget_CubeMAP, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
		m_primitiveIDCubeMap->LoadImage(
		PrimitiveIDTextureWidth,
		PrimitiveIDTextureHeight,
		Null);

		m_primitiveAlbedoCubeMap = std::make_unique<GLTexture>(GLTextureTarget_CubeMAP, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
		m_primitiveAlbedoCubeMap->LoadImage(
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

		if (m_assetManager->sceneMap.find(startSceneName) != m_assetManager->sceneMap.end())
			m_scene = m_assetManager->sceneMap[startSceneName];
		else
			m_scene = std::make_shared<Scene>();

		m_scene->Initialize(m_GLDevice.get(), width, height);

		createBuiltinResources();

		std::shared_ptr<Object> lightObject = createAreaLight();
		lightObject->Initialize(m_GLDevice.get(), True);
		m_scene->AddLight(lightObject, False);

		//	ʵ�������������л��õĶ���
		for (vector<std::shared_ptr<Object>>::iterator iter = m_scene->serializedObjects.begin();
			iter != m_scene->serializedObjects.end();
			++iter)
		{
			m_scene->AddObject(createObject(*iter), False);
		}
		
		m_guiWrapper->Initialize(width, height);

		m_rlShootingPrimitiveBuffer = std::make_unique<RLBuffer>(RLBufferTarget_UniformBlockBuffer);
		m_rlShootingPrimitiveBuffer->name = "ShootingPrimitive";
		m_rlShootingPrimitiveBuffer->AllocateMemorySpace(sizeof(RLPrimitive), RLBufferUsage_DynamicDraw);
	}

	void WindowsEditor::Tick(float deltaTime, int32 width, int32 height, InputState & inputState)
	{
		m_guiWrapper->Tick(width, height, inputState);

		//	Main Window

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;

		bool mainOpened = true;

		ImGui::SetNextWindowPos(ImVec2(400, 800));
		ImGui::SetNextWindowSize(ImVec2(200, 200));

		ImGui::Begin("Main Window", &mainOpened, windowFlags);
		menuBar();
		if (ImGui::Button("GL Reload Shader"))
		{
			m_assetManager->ReloadGLShader();
		}
		if (ImGui::Button("Bake"))
		{
			m_baking = !m_baking;
		}
		
		ImGui::SliderFloat("Thresold: ", &m_thresold, 0, 0.05, "%.6f");
		std::string YString = "Current Y: ";
		YString += to_string(m_currentMaxY);
		ImGui::Text(YString.c_str());

		string BakingInfo = "Frame: ";
		BakingInfo += to_string(m_frameCount - 1);
		ImGui::Text(BakingInfo.c_str());

		if (ImGui::Button("Save Lightmap"))
		{
			Object* BeingBakingObject = m_scene->GetBeingBakingObject();

			int32 RadiosityTextureWidth = BeingBakingObject->glRenderableUnit->staticMesh.lock()->GetRadiosityTextureWidth();
			int32 RadiosityTextureHeight = BeingBakingObject->glRenderableUnit->staticMesh.lock()->GetRadiosityTextureHeight();

			SaveLightmap(BeingBakingObject->name, RadiosityTextureWidth, RadiosityTextureHeight);
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

			m_GLColorAttach = std::make_unique<GLTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA, GLPixelFormat_RGBA, GLDataType_UnsignedByte, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
			m_GLColorAttach->LoadImage(
				rasterizedViewWidth,
				rasterizedViewHeight,
				Null);

			m_GLFrameBuffer->Resize(rasterizedViewWidth, rasterizedViewHeight);
			m_GLFrameBuffer->AttachColor(GLAttachIndexColor0, m_GLColorAttach->GetTarget(), m_GLColorAttach.get());

			m_scene->GetCamera()->ascept = rasterizedRegion.x / rasterizedRegion.y;
			m_scene->GetCamera()->UpdatePerspectiveProjectionMatrix();
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

		ImGui::Begin("CubeMap View");
		ImVec2 DebugViewRegion = ImGui::GetContentRegionAvail();

		int32 DebugViewWidth = static_cast<int32>(DebugViewRegion.x);
		int32 DebugViewHeight = static_cast<int32>(DebugViewRegion.y);

		if (DebugViewWidth != m_GLDebugViewFrameBuffer->GetWidth() ||
			DebugViewHeight != m_GLDebugViewFrameBuffer->GetHeight())
		{
			if (DebugViewWidth <= 0)
				DebugViewWidth = 128;

			if (DebugViewHeight <= 0)
				DebugViewHeight = 128;

			m_GLDebugViewColorAttach = std::make_unique<GLTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA, GLPixelFormat_RGBA, GLDataType_UnsignedByte, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
			m_GLDebugViewColorAttach->LoadImage(
				DebugViewWidth,
				DebugViewHeight,
				Null);

			m_GLDebugViewFrameBuffer->Resize(DebugViewWidth, DebugViewHeight);
			m_GLDebugViewFrameBuffer->AttachColor(GLAttachIndexColor0, m_GLDebugViewColorAttach->GetTarget(), m_GLDebugViewColorAttach.get());

			//	XXX:	这里和GL光栅化的视口尺一致,现在没问题.
			m_scene->GetCamera()->ascept = DebugViewRegion.x / DebugViewRegion.y;
			m_scene->GetCamera()->UpdatePerspectiveProjectionMatrix();
		}

		//if (m_RadiosityTexture.get())
		//{
		//	ImGui::Image(
		//		reinterpret_cast<void *>(m_RadiosityTexture->GetID64()),
		//		DebugViewRegion,
		//		ImVec2(0, 1.0f),
		//		ImVec2(1.0f, 0));
		//}
		//else
		//{
		//	ImGui::Image(
		//		reinterpret_cast<void *>(m_GLDebugViewColorAttach->GetID64()),
		//		DebugViewRegion,
		//		ImVec2(0, 1.0f),
		//		ImVec2(1.0f, 0));
		//}

		if (m_GLNormalAttach)
		{
			ImGui::Image(
				reinterpret_cast<void *>(m_GLNormalAttach->GetID64()),
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

			ImGui::Begin("Radiosity View");
			ImVec2 ProfileViewRegion = ImGui::GetContentRegionAvail();
		
			//ImGui::Image(
			//	reinterpret_cast<void *>(
			//	(m_frameCount - 1) % 2 == 0 ? m_RadiosityImage1->GetID64() : m_RadiosityImage0->GetID64()),
			//	ProfileViewRegion,
			//	ImVec2(0, 1.0f),
			//	ImVec2(1.0f, 0));
//
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
		m_scene->Render(m_GLDevice.get(), m_GLFrameBuffer->GetWidth(), m_GLFrameBuffer->GetHeight());
		m_GLFrameBuffer->Inactivate();
		
		m_guiWrapper->Render(width, height);
	}

	void WindowsEditor::ViewCubeMap()
	{
		m_GLDebugViewFrameBuffer->Activate();
		m_GLDevice->BeginViewCubeMapPass(m_GLDebugViewFrameBuffer->GetWidth(), m_GLDebugViewFrameBuffer->GetHeight());
		m_scene->GetCamera()->UpdataGLParam(m_GLDevice.get());
		Object* BeingBakingObject = m_scene->GetBeingBakingObject();
		BeingBakingObject->glRenderableUnit->ViewCubeMapMaterial.lock()->IDCumeMap = m_primitiveAlbedoCubeMap;
		m_GLDevice->UploadGlobalShaderData(GLShaderDataAlias_CubeMatrices, sizeof(CubeMatrices), &CubeMatrices);
		BeingBakingObject->ViewCubeMap(m_GLDevice.get());
		m_GLDebugViewFrameBuffer->Inactivate();
	}
	
	void WindowsEditor::Bake()
	{
		if (!m_baking)
			return;

		static queue<Primitive> RemainingPrimitives;

		if (RemainingPrimitives.empty())
		{
			m_frameCount = 0;
		}

		//	TODO:	这里需要"聚合"的封装,目前只实现对单个对象的bake.
		Object* BeingBakingObject = m_scene->GetBeingBakingObject();
		
		if (m_frameCount == 0)
		{
			BeingBakingObject->BeforeBaking();
			BeingBakingObject->glRenderableUnit->ComputeFormFactorMaterial.lock()->albedoTexture = BeingBakingObject->glRenderableUnit->material.lock()->albedoTexture;
			BeingBakingObject->glRenderableUnit->ComputeFormFactorMaterial.lock()->IDCumeMap = m_primitiveIDCubeMap;
			BeingBakingObject->glRenderableUnit->ComputeFormFactorMaterial.lock()->VisibilityTexture = m_GLVisibilityTexture;

			int32 RadiosityTextureWidth = BeingBakingObject->glRenderableUnit->staticMesh.lock()->GetRadiosityTextureWidth();
			int32 RadiosityTextureHeight =BeingBakingObject->glRenderableUnit->staticMesh.lock()->GetRadiosityTextureHeight();
			
			m_RadiosityTexture = std::make_unique<GLTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
			m_RadiosityTexture->LoadImage(
			RadiosityTextureWidth,
			RadiosityTextureHeight,
			Null);

			float* BlackData = new float[RadiosityTextureWidth * RadiosityTextureHeight * 4];
			for (int32 i = 0; i < RadiosityTextureWidth * RadiosityTextureHeight; ++i)
			{
				BlackData[i * 4 + 0] = 0;
				BlackData[i * 4 + 1] = 0;
				BlackData[i * 4 + 2] = 0;
				BlackData[i * 4 + 3] = 0;
			}
			
			m_RadiosityImage0 = std::make_shared<GLImageTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point, GLImageUnit_0, 0, GLImageAccess_ReadWrite);
			m_RadiosityImage0->LoadImage(
			RadiosityTextureWidth,
			RadiosityTextureHeight,
			BlackData);
			BeingBakingObject->glRenderableUnit->ComputeFormFactorMaterial.lock()->RadiosityImage0 = m_RadiosityImage0;
			
			m_RadiosityImage1 = std::make_shared<GLImageTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point, GLImageUnit_1, 0, GLImageAccess_ReadWrite);
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
			
			m_reconstructionPassFrameBuffer = std::make_unique<GLFrameBuffer>();
			m_reconstructionPassFrameBuffer->Resize(RadiosityTextureWidth, RadiosityTextureHeight);
			m_reconstructionPassFrameBuffer->AttachColor(GLAttachIndexColor0, m_RadiosityTexture->GetTarget(), m_RadiosityTexture.get());

			m_pRadiosityImageRawData = new float[RadiosityTextureWidth * RadiosityTextureHeight * sizeof(float) * 4];
			memset(m_pRadiosityImageRawData, 0, RadiosityTextureWidth * RadiosityTextureHeight * sizeof(float) * 4);
			
			m_pResidualImageRawData = new float[RadiosityTextureWidth * RadiosityTextureHeight * sizeof(float) * 4];
			memset(m_pResidualImageRawData, 0, RadiosityTextureWidth * RadiosityTextureHeight * sizeof(float) * 4);
			
			Object* AreaLight = m_scene->GetAreaLight();
			AreaLight->BeforeBaking();
			for (map<int32, Primitive>::iterator iter = AreaLight->glRenderableUnit.get()->staticMesh.lock().get()->PrimitiveMap.begin();
					iter != AreaLight->glRenderableUnit.get()->staticMesh.lock().get()->PrimitiveMap.end();
					++iter)
			{
				//	TODO:	这里光源的强度先写死.要实现支持W,还有cd/m^2.
				iter->second.Energy = Vector4(3.0, 3.0, 3.0, 1.0);
				iter->second.LightPrimitive = True;
				RemainingPrimitives.push(iter->second);
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
			float* m_pPrimitiveIDRawData = new float[RadiosityTextureWidth * RadiosityTextureHeight * 4];
			
			m_GLPositionAttach->Fetch(m_pPositionRawData);
			m_GLNormalAttach->Fetch(m_pNormalRawData);
			
			m_RLBakingObjectPosition->LoadImage(RadiosityTextureWidth, RadiosityTextureHeight, m_pPositionRawData);
			m_RLBakingObjectNormal->LoadImage(RadiosityTextureWidth, RadiosityTextureHeight, m_pNormalRawData);
		
			delete[] m_pPositionRawData;
			delete[] m_pNormalRawData;
			delete[] m_pPrimitiveIDRawData;
			
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

			std::unique_ptr<Object> PrimitiveObject;
			
			if (!ShootingPrimitive.LightPrimitive)
			{
				BeingBakingObject->SetShootingPrimitive(ShootingPrimitive.ID);
			}
			else
			{
				PrimitiveObject = CreateObject(ShootingPrimitive);
				BeingBakingObject->SetShootingPrimitive(-1.0f);
			}
			
			BeingBakingObject->glRenderableUnit->DrawIDMaterial.lock()->albedoTexture = m_assetManager->textureMap["Sponza_Bricks_a_Albedo"];
			
			//	RL Visibility
			{
				int32 RadiosityTextureWidth = BeingBakingObject->glRenderableUnit->staticMesh.lock()->GetRadiosityTextureWidth();
				int32 RadiosityTextureHeight = BeingBakingObject->glRenderableUnit->staticMesh.lock()->GetRadiosityTextureHeight();
				
				m_rlShootingPrimitiveBuffer->Activate();
				RLPrimitive* RLPrimitiveData = (RLPrimitive*)m_rlShootingPrimitiveBuffer->Map(RLBufferAccessFlag_ReadWrite);
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
				m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor1, GLTextureTarget_CubeMap_Positive_X, m_primitiveAlbedoCubeMap.get());
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
				m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor1, GLTextureTarget_CubeMap_Negative_X, m_primitiveAlbedoCubeMap.get());
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
				m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor1, GLTextureTarget_CubeMap_Positive_Y, m_primitiveAlbedoCubeMap.get());
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
				m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor1, GLTextureTarget_CubeMap_Negative_Y, m_primitiveAlbedoCubeMap.get());
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
				m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor1, GLTextureTarget_CubeMap_Positive_Z, m_primitiveAlbedoCubeMap.get());
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
				m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor1, GLTextureTarget_CubeMap_Negative_Z, m_primitiveAlbedoCubeMap.get());
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

			//	Reconstruction Pass
			m_reconstructionPassFrameBuffer->Activate();
			{
				int32 RadiosityTextureWidth = BeingBakingObject->glRenderableUnit->staticMesh.lock()->GetRadiosityTextureWidth();
				int32 RadiosityTextureHeight = BeingBakingObject->glRenderableUnit->staticMesh.lock()->GetRadiosityTextureHeight();
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
				for (int32 PrimitiveIndex = 0; PrimitiveIndex < BeingBakingObject->glRenderableUnit->staticMesh.lock()->indexCount / 3; ++PrimitiveIndex)
				{
					Primitive BakingPrimitive = BeingBakingObject->glRenderableUnit->staticMesh.lock()->PrimitiveMap[PrimitiveIndex];
					
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
					BeingBakingObject->glRenderableUnit->staticMesh.lock()->CalculateOrthoParameters(
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

				int32 RadiosityTextureWidth = BeingBakingObject->glRenderableUnit->staticMesh.lock()->GetRadiosityTextureWidth();
				int32 RadiosityTextureHeight = BeingBakingObject->glRenderableUnit->staticMesh.lock()->GetRadiosityTextureHeight();

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
				float ScaledUV1Area = BeingBakingObject->glRenderableUnit->staticMesh.lock()->PrimitiveMap[MaxYPrimitiveID].ScaledUV1Area;
				RGB.x /= ScaledUV1Area;
				RGB.y /= ScaledUV1Area;
				RGB.z /= ScaledUV1Area;
				m_currentMaxY = RGB.r * 0.30f + RGB.g * 0.59f + RGB.b * 0.11f;

				if (m_currentMaxY < m_thresold)
				{
					m_baking = False;
					return ;
				}
				
				BeingBakingObject->glRenderableUnit->staticMesh.lock()->PrimitiveMap[MaxYPrimitiveID].Energy = Vector4(RGB.x, RGB.y, RGB.z, 1.0);
				RemainingPrimitives.push(BeingBakingObject->glRenderableUnit->staticMesh.lock()->PrimitiveMap[MaxYPrimitiveID]);

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

		//m_baking = False;
		
		//m_frameCount = 0;
		//while (!RemainingPrimitives.empty())
		//{
		//	RemainingPrimitives.pop();
		//}
	}

	WindowsEditor::~WindowsEditor()
	{
		delete[] m_pRadiosityImageRawData;
		delete[] m_pResidualImageRawData;
	}
}