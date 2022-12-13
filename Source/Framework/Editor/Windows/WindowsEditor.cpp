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

		m_arealLightMaterial->lightmapName = "DummyLightmap";
		m_arealLightMaterial->lightmapTexture = m_assetManager->lightmapMap["DummyLightmap"];
		m_arealLightMaterial->lightmapTexture.lock()->Attach(m_arealLightMaterial.get());
		m_arealLightMaterial->IsOccluder = False;

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

		m_areaLightMesh = std::make_shared<StaticMesh>();

		m_areaLightMesh->vertexCount = 6;
		m_areaLightMesh->pPositions = new Vector4[m_areaLightMesh->vertexCount];
		m_areaLightMesh->pNormals = new Vector3[m_areaLightMesh->vertexCount];
		m_areaLightMesh->pUV0s = new Vector2[m_areaLightMesh->vertexCount];
		
		float lightScale = 10.0f;

		m_areaLightMesh->pPositions[0].x = -0.5f * lightScale;
		m_areaLightMesh->pPositions[0].y = -0.5f * lightScale ;
		m_areaLightMesh->pPositions[0].z = 0;
		m_areaLightMesh->pPositions[0].w = 1.0f;
		m_areaLightMesh->pNormals[0].x = 0;
		m_areaLightMesh->pNormals[0].y = 0;
		m_areaLightMesh->pNormals[0].z = 1.0f;
		m_areaLightMesh->pUV0s[0] = Vector2(0, 0);

		m_areaLightMesh->pPositions[1].x = 0.5f * lightScale;
		m_areaLightMesh->pPositions[1].y = -0.5f * lightScale ;
		m_areaLightMesh->pPositions[1].z = 0;
		m_areaLightMesh->pPositions[1].w = 1.0f;
		m_areaLightMesh->pNormals[1].x = 0;
		m_areaLightMesh->pNormals[1].y = 0;
		m_areaLightMesh->pNormals[1].z = 1.0f;
		m_areaLightMesh->pUV0s[1] = Vector2(1.0, 0);

		m_areaLightMesh->pPositions[2].x = 0.5f * lightScale;
		m_areaLightMesh->pPositions[2].y = 0.5f * lightScale ;
		m_areaLightMesh->pPositions[2].z = 0;
		m_areaLightMesh->pPositions[2].w = 1.0f;
		m_areaLightMesh->pNormals[2].x = 0;
		m_areaLightMesh->pNormals[2].y = 0;
		m_areaLightMesh->pNormals[2].z = 1.0f;
		m_areaLightMesh->pUV0s[2] = Vector2(1.0f, 1.0f);

		m_areaLightMesh->pPositions[3].x = 0.5f * lightScale;
		m_areaLightMesh->pPositions[3].y = 0.5f * lightScale ;
		m_areaLightMesh->pPositions[3].z = 0;
		m_areaLightMesh->pPositions[3].w = 1.0f;
		m_areaLightMesh->pNormals[3].x = 0;
		m_areaLightMesh->pNormals[3].y = 0;
		m_areaLightMesh->pNormals[3].z = 1.0f;
		m_areaLightMesh->pUV0s[3] = Vector2(1.0f, 1.0f);

		m_areaLightMesh->pPositions[4].x = -0.5f * lightScale;
		m_areaLightMesh->pPositions[4].y = 0.5f * lightScale ;
		m_areaLightMesh->pPositions[4].z = 0;
		m_areaLightMesh->pPositions[4].w = 1.0f;
		m_areaLightMesh->pNormals[4].x = 0;
		m_areaLightMesh->pNormals[4].y = 0;
		m_areaLightMesh->pNormals[4].z = 1.0f;
		m_areaLightMesh->pUV0s[4] = Vector2(0, 1.0f);

		m_areaLightMesh->pPositions[5].x = -0.5f * lightScale;
		m_areaLightMesh->pPositions[5].y = -0.5f * lightScale ;
		m_areaLightMesh->pPositions[5].z = 0;
		m_areaLightMesh->pPositions[5].w = 1.0f;
		m_areaLightMesh->pNormals[5].x = 0;
		m_areaLightMesh->pNormals[5].y = 0;
		m_areaLightMesh->pNormals[5].z = 1.0f;
		m_areaLightMesh->pUV0s[5] = Vector2(0, 0);

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
		defaultObject->glRenderableUnit->DrawIDMaterial = m_DrawIDMaterial;
		defaultObject->glRenderableUnit->ComputeFormFactorMaterial = m_ComputeFormFactorMaterial;

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
		object->glRenderableUnit->DrawIDMaterial = m_DrawIDMaterial;
		object->glRenderableUnit->ComputeFormFactorMaterial = m_ComputeFormFactorMaterial;

		object->name = object->staticMeshName;

		object->Initialize(m_GLDevice.get(), True);

		return object;
	}

	std::shared_ptr<Core::Object> WindowsEditor::createAreaLight()
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

	WindowsEditor::WindowsEditor()
		:
		m_assetManager(std::make_unique<AssetManager>()),
		m_GLDevice(std::make_unique<OpenGLDevice>()),
		m_guiWrapper(std::make_unique<GUIWrapper>()),
		m_GLFrameBuffer(std::make_unique<GLFrameBuffer>()),
		m_frameCount(0),
		m_baking(False)
	{
		//	Visibility Pass
		m_primitiveIDTexture = std::make_unique<GLTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
		m_primitiveIDTexture->LoadImage(
		PrimitiveIDTextureWidth,
		PrimitiveIDTextureHeight,
		Null);

		m_visibilityPassFrameBuffer = std::make_unique<GLFrameBuffer>();
		m_visibilityPassFrameBuffer->Resize(PrimitiveIDTextureWidth, PrimitiveIDTextureHeight);
		m_visibilityPassFrameBuffer->AttachColor(GLAttachIndexColor0, m_primitiveIDTexture.get());
		
		m_pPrimitiveIDRawData = new float[PrimitiveIDTextureWidth * PrimitiveIDTextureHeight * sizeof(float) * 4];
		memset(m_pPrimitiveIDRawData, 0, PrimitiveIDTextureWidth * PrimitiveIDTextureHeight * sizeof(float) * 4);
	}

	void WindowsEditor::Initialize(int32 width, int32 height)
	{
		m_assetManager->Scan();

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

		//ImGui::InputInt("Progressive Count:", &debugingProgressiveCount);

		//ctd::string bakingInfo = "Progressive[";
		//bakingInfo += to_string(m_frameCount);
		//bakingInfo += "/";
		//bakingInfo += to_string(debugingProgressiveCount);// LightmappingSetting::Instance()->progressivePassCount);
		//bakingInfo += "]";
		//ImGui::Text(bakingInfo.c_str());

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
			m_GLFrameBuffer->AttachColor(GLAttachIndexColor0, m_GLColorAttach.get());

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
				m_scene->GetCamera()->GetPerspectivePeojectionMatrix(),
				m_pSelectedObject);
		}

		ImGui::End();
		
		//////////////////////////////////////////////////////////////////////////
		if (m_RadiorityTexture.get())
		{
			ImGui::SetNextWindowPos(ImVec2(600, 0));
			ImGui::SetNextWindowSize(ImVec2(600, 600));
		
			ImGui::Begin("Pfofile View");
			ImVec2 rayTracedRegion = ImGui::GetContentRegionAvail();
		
			ImGui::Image(
				reinterpret_cast<void *>(m_RadiorityTexture->GetID64()),
				rayTracedRegion,
				ImVec2(0, 1.0f),
				ImVec2(1.0f, 0));

			ImGui::End();
		}
		
		//////////////////////////////////////////////////////////////////////////
		ImGui::SetNextWindowPos(ImVec2(0, 600));
		ImGui::SetNextWindowSize(ImVec2(200, 200));
		
		ImGui::Begin("Color Attach 0");
		ImVec2 colorAttach0Region = ImGui::GetContentRegionAvail();
		
		ImGui::Image(
			reinterpret_cast<void *>(m_primitiveIDTexture->GetID64()),
			colorAttach0Region,
			ImVec2(0, 1.0f),
			ImVec2(1.0f, 0));
		
		ImGui::End();
		//////////////////////////////////////////////////////////////////////////
		ImGui::SetNextWindowPos(ImVec2(200, 600));
		ImGui::SetNextWindowSize(ImVec2(200, 200));
		
		ImGui::Begin("Color Attach 1");
		ImVec2 colorAttach1Region = ImGui::GetContentRegionAvail();
		
		ImGui::Image(
			reinterpret_cast<void *>(m_primitiveIDTexture->GetID64()),
			colorAttach1Region,
			ImVec2(0, 1.0f),
			ImVec2(1.0f, 0));
		
		ImGui::End();
		//////////////////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////////////////////////////////
		if (m_RadiorityTexture.get())
		{
			ImGui::SetNextWindowPos(ImVec2(0, 800));
			ImGui::SetNextWindowSize(ImVec2(200, 200));
		
			ImGui::Begin("Bake View");
			ImVec2 bakeViewRegion = ImGui::GetContentRegionAvail();
		
			ImGui::Image(
				reinterpret_cast<void *>(m_RadiorityTexture->GetID64()),
				bakeViewRegion,
				ImVec2(0, 1.0f),
				ImVec2(1.0f, 0));
		
			ImGui::End();
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		if (m_ResidualTexture.get())
		{
			ImGui::SetNextWindowPos(ImVec2(200, 800));
			ImGui::SetNextWindowSize(ImVec2(200, 200));

			ImGui::Begin("Debug View");
			ImVec2 debugViewRegion = ImGui::GetContentRegionAvail();

			ImGui::Image(
				reinterpret_cast<void *>(m_ResidualTexture->GetID64()),
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

	void WindowsEditor::Bake()
	{
		if (!m_baking)
			return;

		//	TODO:	这里需要"聚合"的封装,目前只实现对单个对象的bake.
		Object* BeingBakingObject = m_scene->GetBeingBakingObject();

		static queue<Primitive> RemainingPrimitives;
		
		if (m_frameCount == 0)
		{
			BeingBakingObject->BeforeBaking();

			int32 RadiosityTextureWidth = BeingBakingObject->glRenderableUnit->staticMesh.lock().get()->GetRadiosityTextureWidth();
			int32 RadiosityTextureHeight =BeingBakingObject->glRenderableUnit->staticMesh.lock().get()->GetRadiosityTextureHeight();
			
			m_RadiorityTexture = std::make_unique<GLTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
			m_RadiorityTexture->LoadImage(
			RadiosityTextureWidth,
			RadiosityTextureHeight,
			Null);

			m_ResidualTexture = std::make_unique<GLTexture>(GLTextureTarget_2D, GLInternalFormat_RGBA32F, GLPixelFormat_RGBA, GLDataType_Float, GLTextureWrapMode_Clamp, GLTextureFilterMode_Point);
			m_ResidualTexture->LoadImage(
			RadiosityTextureWidth,
			RadiosityTextureHeight,
			Null);
			
			m_reconstructionPassFrameBuffer = std::make_unique<GLFrameBuffer>();
			m_reconstructionPassFrameBuffer->Resize(RadiosityTextureWidth, RadiosityTextureHeight);
			m_reconstructionPassFrameBuffer->AttachColor(GLAttachIndexColor0, m_RadiorityTexture.get());
			m_reconstructionPassFrameBuffer->AttachColor(GLAttachIndexColor1, m_ResidualTexture.get());

			Object* AreaLight = m_scene->GetAreaLight();
			AreaLight->BeforeBaking();
			
			for (map<int32, Primitive>::iterator iter = AreaLight->glRenderableUnit.get()->staticMesh.lock().get()->PrimitiveMap.begin();
					iter != AreaLight->glRenderableUnit.get()->staticMesh.lock().get()->PrimitiveMap.end();
					++iter)
			{
				//	TODO:	这里光源的强度先写死.要实现支持W,还有cd/m^2.
				iter->second.Energy = Vector3(1.0, 1.0, 1.0);
				RemainingPrimitives.push(iter->second);
			}
		}
		
		if (!RemainingPrimitives.empty())
		{
			Primitive Primitive = RemainingPrimitives.front();
			RemainingPrimitives.pop();
			
			static Camera Camera;
			Camera.zNear = 1.0f;
			Camera.zFar = 6000.0f;
			Camera.ascept = 1.0f;
			Camera.ascept *= static_cast<float>(PrimitiveIDTextureWidth);
			Camera.ascept /= static_cast<float>(PrimitiveIDTextureHeight);
			Camera.fovY = 120.0f * Deg2Rad;
			Camera.position = Primitive.ShootPosition;
			Camera.lookAt = Primitive.Normal;
			Camera.UpdateViewMatrix();
			float Left;
			float Right;
			float Bottom;
			float Top;
			float ZNear;
			float ZFar;
			BeingBakingObject->glRenderableUnit.get()->staticMesh.lock()->CalculateOrthoParameters(
				*BeingBakingObject->GetObject2WorldMatrix(),
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
			Camera.UpdateMatrix();
			Camera.UpdataGLParam(m_GLDevice.get());
			
			//	Visibility Pass
			m_visibilityPassFrameBuffer->Activate();
			{
				m_GLDevice->BeginVisibisityPass(PrimitiveIDTextureWidth, PrimitiveIDTextureHeight);
				BeingBakingObject->DrawID(m_GLDevice.get());
			}
			m_visibilityPassFrameBuffer->Inactivate();

			//	Reconstruction Pass
			m_reconstructionPassFrameBuffer->Activate();
			{
				int32 RadiosityTextureWidth = BeingBakingObject->glRenderableUnit->staticMesh.lock().get()->GetRadiosityTextureWidth();
				int32 RadiosityTextureHeight =BeingBakingObject->glRenderableUnit->staticMesh.lock().get()->GetRadiosityTextureHeight();
				m_GLDevice->BeginReconstrucionPass(RadiosityTextureWidth, RadiosityTextureHeight);
				ShooterInfo ShooterInfo;
				ShooterInfo.Position = Primitive.ShootPosition;
				ShooterInfo.Normal = Primitive.Normal;
				ShooterInfo.Energy = Primitive.Energy;
				ShooterInfo.SurfaceArea = Vector3(Primitive.SurfaceArea, Primitive.SurfaceArea, Primitive.SurfaceArea);
				m_GLDevice->UploadGlobalShaderData(GLShaderDataAlias_ShooterInfo, sizeof(ShooterInfo), &ShooterInfo);
				BeingBakingObject->ComputeFormFactor(m_GLDevice.get());
			}
			m_reconstructionPassFrameBuffer->Inactivate();
			
			if (RemainingPrimitives.empty())
			{
				//	pick next emmiter
			}
		}
		
		++m_frameCount;

		if (RemainingPrimitives.empty())
		{
			m_frameCount = 0;
		}
		
		m_baking = False;

		//m_primitiveIDTexture->Fetch(m_pPrimitiveIDRawData);
		
		//	��ȾGI
		if (0)
		{
			//////////////////////////////////////////////////////////////////////////
			
			if (m_frameCount == 0)
				m_assetManager->ScanLightmap();

			m_assetManager->ReloadLightmap();

			for (vector<std::shared_ptr<Object>>::iterator iter = m_scene->objects.begin();
				iter != m_scene->objects.end();
				++iter)
			{
				//if ((*iter)->glRenderableUnit->bakingMaterial.expired())
				//	continue;
			
				//string lightmapName = LightmappingSetting::Instance()->GetLightmapName((*iter)->glRenderableUnit->material.lock()->lightmapIndex);
				//(*iter)->glRenderableUnit->material.lock()->lightmapName = lightmapName;
			//
				//(*iter)->glRenderableUnit->material.lock()->lightmapTexture = m_assetManager->lightmapMap[lightmapName];
				//(*iter)->glRenderableUnit->material.lock()->lightmapTexture.lock()->BeginUse();
			}
		}
	}

	WindowsEditor::~WindowsEditor()
	{
		delete[] m_pPrimitiveIDRawData;
	}
}