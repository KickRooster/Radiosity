#include <BaseInclude.h>
#include "FBXImportor.h"
#include <fbxsdk\scene\geometry\fbxlayer.h>
#include SET_INCLUDE_PATH

namespace Core
{
#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif

	ErrorCode FBXImportor::initializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
	{
		//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
		pManager = FbxManager::Create();

		if (!pManager)
		{
			FBXSDK_printf("Error: Unable to create FBX Manager!\n");
			return !ErrorCode_OK;
		}
		else
			FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

		//Create an IOSettings object. This object holds all import/export settings.
		FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
		pManager->SetIOSettings(ios);

		//Load plugins from the executable directory (optional)
		FbxString lPath = FbxGetApplicationDirectory();
		pManager->LoadPluginsDirectory(lPath.Buffer());

		//Create an FBX scene. This object holds most objects imported/exported from/to files.
		pScene = FbxScene::Create(pManager, "My Scene");
		
		if (!pScene)
		{
			FBXSDK_printf("Error: Unable to create FBX scene!\n");
			return !ErrorCode_OK;
		}

		return ErrorCode_OK;
	}

	ErrorCode FBXImportor::loadScene(FbxManager* pManager, FbxDocument* pScene, const ANSICHAR* pFilename)
	{
		int32 lFileMajor, lFileMinor, lFileRevision;
		int32 lSDKMajor, lSDKMinor, lSDKRevision;
		Bool lStatus;

		// Get the file version number generate by the FBX SDK.
		FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

		// Create an importer.
		FbxImporter* lImporter = FbxImporter::Create(pManager, "");

		// Initialize the importer by providing a filename.
		const Bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
		lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

		if (!lImportStatus)
		{
			FbxString error = lImporter->GetStatus().GetErrorString();
			FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
			FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

			if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
			{
				FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
				FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
			}

			return !ErrorCode_OK;
		}

		FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

		if (lImporter->IsFBX())
		{
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

			// Set the import states. By default, the import states are always set to 
			// true. The code below shows how to change these states.
			IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
			IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
			IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
			IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
			IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
			IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
			IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
		}

		// Import the scene.
		lStatus = lImporter->Import(pScene);

		if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
			return !ErrorCode_OK;

		// Destroy the importer.
		lImporter->Destroy();

		return lStatus ? ErrorCode_OK : !ErrorCode_OK ;
	}

	void FBXImportor::processMesh(FbxNode* pNode, ctd::vector<std::unique_ptr<StaticMesh>> & outStaticMeshes, ctd::vector<std::unique_ptr<Material>> & outMaterials, Bool generateTangentsData /*= True*/)
	{
		ctd::string nodeName = pNode->GetName();

		FbxMesh* pMesh = (FbxMesh*)pNode->GetNodeAttribute();

		FbxAnimEvaluator* pAnimEvaluator = pNode->GetScene()->GetAnimationEvaluator();
 
		FbxAMatrix & globalTransform = pAnimEvaluator->GetNodeGlobalTransform(pNode);

		Matrix4x4 local2World;

		local2World[0][0] = static_cast<float>(globalTransform[0][0]);
		local2World[0][1] = static_cast<float>(globalTransform[0][1]);
		local2World[0][2] = static_cast<float>(globalTransform[0][2]);
		local2World[0][3] = static_cast<float>(globalTransform[0][3]);

		local2World[1][0] = static_cast<float>(globalTransform[1][0]);
		local2World[1][1] = static_cast<float>(globalTransform[1][1]);
		local2World[1][2] = static_cast<float>(globalTransform[1][2]);
		local2World[1][3] = static_cast<float>(globalTransform[1][3]);

		local2World[2][0] = static_cast<float>(globalTransform[2][0]);
		local2World[2][1] = static_cast<float>(globalTransform[2][1]);
		local2World[2][2] = static_cast<float>(globalTransform[2][2]);
		local2World[2][3] = static_cast<float>(globalTransform[2][3]);

		local2World[3][0] = static_cast<float>(globalTransform[3][0]);
		local2World[3][1] = static_cast<float>(globalTransform[3][1]);
		local2World[3][2] = static_cast<float>(globalTransform[3][2]);
		local2World[3][3] = static_cast<float>(globalTransform[3][3]);

		if (generateTangentsData)
			pMesh->GenerateTangentsData(0);

		int32 indexCount = pMesh->GetPolygonVertexCount();
		int32 vertexCount = pMesh->GetPolygonVertexCount();
		int32 materialCount = pMesh->GetElementMaterialCount();

		//	XXX:
		//		�ڴ˺����������Ѿ����������ݲ��ʻ���mesh�ķ���
		//		�ڴ˼ٶ���ǰmesh��ֻ��1�ֲ���
		assert(materialCount == 1);

		FbxGeometryElementMaterial* pRawMaterial = pMesh->GetElementMaterial(0);

		std::unique_ptr<Material> pMaterial(new Material());
		pMaterial->fileName = nodeName;
		
		outMaterials.push_back(std::move(pMaterial));

		std::unique_ptr<StaticMesh> pStaticMesh(new StaticMesh());
		pStaticMesh->fileName = nodeName;

		pStaticMesh->SetLocal2World(local2World);
		
		if (pStaticMesh->pIndices != Null)
			delete[] pStaticMesh->pIndices;

		pStaticMesh->pIndices = new uint32[indexCount];
		
		if (pStaticMesh->pPositions != Null)
			delete[] pStaticMesh->pPositions;

		pStaticMesh->pPositions = new Vector4[vertexCount];

		int32 colorChannelSize = pMesh->GetElementVertexColorCount();

		if (colorChannelSize > 0)
		{
			if (pStaticMesh->pColors != Null)
				delete[] pStaticMesh->pColors;

			pStaticMesh->pColors = new Vector4[vertexCount];
		}

		int32 uvChannelCount = pMesh->GetElementUVCount();

		if (uvChannelCount > 0)
		{
			if (pStaticMesh->pUV0s != Null)
				delete[] pStaticMesh->pUV0s;

			if (pStaticMesh->pUV1s != Null)
				delete[] pStaticMesh->pUV1s;

			if (pStaticMesh->pUV2s != Null)
				delete[] pStaticMesh->pUV2s;

			if (pStaticMesh->pUV3s != Null)
				delete[] pStaticMesh->pUV3s;

			if (pStaticMesh->pUV4s != Null)
				delete[] pStaticMesh->pUV4s;

			if (pStaticMesh->pUV5s != Null)
				delete[] pStaticMesh->pUV5s;

			if (pStaticMesh->pUV6s != Null)
				delete[] pStaticMesh->pUV6s;

			if (pStaticMesh->pUV7s != Null)
				delete[] pStaticMesh->pUV7s;

			pStaticMesh->pUV0s = new Vector2[vertexCount];

			if (uvChannelCount > 1)
				pStaticMesh->pUV1s = new Vector2[vertexCount];

			if (uvChannelCount > 2)
				pStaticMesh->pUV2s = new Vector2[vertexCount];

			if (uvChannelCount > 3)
				pStaticMesh->pUV3s = new Vector2[vertexCount];

			if (uvChannelCount > 4)
				pStaticMesh->pUV4s = new Vector2[vertexCount];

			if (uvChannelCount > 5)
				pStaticMesh->pUV5s = new Vector2[vertexCount];

			if (uvChannelCount > 6)
				pStaticMesh->pUV6s = new Vector2[vertexCount];

			if (uvChannelCount > 7)
				pStaticMesh->pUV7s = new Vector2[vertexCount];
		}

		int32 normalChannelCount = pMesh->GetElementNormalCount();

		if (normalChannelCount > 0)
		{
			if (pStaticMesh->pNormals != Null)
				delete[] pStaticMesh->pNormals;

			pStaticMesh->pNormals = new Vector3[vertexCount];
		}

		int32 tangentChannelCount = pMesh->GetElementTangentCount();

		if (tangentChannelCount > 0)
		{
			if (pStaticMesh->pTangents != Null)
				delete[] pStaticMesh->pTangents;

			pStaticMesh->pTangents = new Vector3[vertexCount];
		}

		int32 binormalChannelCount = pMesh->GetElementBinormalCount();

		if (binormalChannelCount > 0)
		{
			if (pStaticMesh->pBinormals != Null)
				delete[] pStaticMesh->pBinormals;

			pStaticMesh->pBinormals = new Vector3[vertexCount];
		}

		//////////////////////////////////////////////////////////////////////////

		int32 * pTriangles = pMesh->GetPolygonVertices();

		pStaticMesh->vertexCount = vertexCount;

		int32 * pControlPointIndices = new int32[vertexCount];

		//////////////////////////////////////////////////////////////////////////
		for (int32 vertexID = 0; vertexID < vertexCount; ++vertexID)
		{
			int32 controlPointIndex = pTriangles[vertexID];

			pControlPointIndices[vertexID] = controlPointIndex;

			FbxVector4 controlPoint = pMesh->GetControlPoints()[controlPointIndex];

			pStaticMesh->pPositions[vertexID].x = (float)controlPoint[0];
			pStaticMesh->pPositions[vertexID].y = (float)controlPoint[1];
			pStaticMesh->pPositions[vertexID].z = (float)controlPoint[2];
			pStaticMesh->pPositions[vertexID].w = 1.0f;

			if (colorChannelSize > 0)
			{
				FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(0);

				if (leVtxc->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					if (leVtxc->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						pStaticMesh->pColors[vertexID].r = (float)leVtxc->GetDirectArray().GetAt(controlPointIndex).mRed;
						pStaticMesh->pColors[vertexID].g = (float)leVtxc->GetDirectArray().GetAt(controlPointIndex).mGreen;
						pStaticMesh->pColors[vertexID].b = (float)leVtxc->GetDirectArray().GetAt(controlPointIndex).mBlue;
						pStaticMesh->pColors[vertexID].a = (float)leVtxc->GetDirectArray().GetAt(controlPointIndex).mAlpha;
					}
					else if (leVtxc->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int32 id = leVtxc->GetIndexArray().GetAt(controlPointIndex);

						pStaticMesh->pColors[vertexID].r = (float)leVtxc->GetDirectArray().GetAt(id).mRed;
						pStaticMesh->pColors[vertexID].g = (float)leVtxc->GetDirectArray().GetAt(id).mGreen;
						pStaticMesh->pColors[vertexID].b = (float)leVtxc->GetDirectArray().GetAt(id).mBlue;
						pStaticMesh->pColors[vertexID].a = (float)leVtxc->GetDirectArray().GetAt(id).mAlpha;
					}
				}
				else if (leVtxc->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					if (leVtxc->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						pStaticMesh->pColors[vertexID].r = (float)leVtxc->GetDirectArray().GetAt(vertexID).mRed;
						pStaticMesh->pColors[vertexID].g = (float)leVtxc->GetDirectArray().GetAt(vertexID).mGreen;
						pStaticMesh->pColors[vertexID].b = (float)leVtxc->GetDirectArray().GetAt(vertexID).mBlue;
						pStaticMesh->pColors[vertexID].a = (float)leVtxc->GetDirectArray().GetAt(vertexID).mAlpha;
					}
					else if (leVtxc->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int32 id = leVtxc->GetIndexArray().GetAt(vertexID);
						pStaticMesh->pColors[vertexID].r = (float)leVtxc->GetDirectArray().GetAt(id).mRed;
						pStaticMesh->pColors[vertexID].g = (float)leVtxc->GetDirectArray().GetAt(id).mGreen;
						pStaticMesh->pColors[vertexID].b = (float)leVtxc->GetDirectArray().GetAt(id).mBlue;
						pStaticMesh->pColors[vertexID].a = (float)leVtxc->GetDirectArray().GetAt(id).mAlpha;
					}
				}
				else
				{

				}
			}

			for (int32 k = 0; k < uvChannelCount; ++k)
			{
				FbxGeometryElementUV* leUV = pMesh->GetElementUV(k);

				Vector2 uv;

				if (leUV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					if (leUV->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						uv.x = (float)leUV->GetDirectArray().GetAt(controlPointIndex)[0];
						uv.y = (float)leUV->GetDirectArray().GetAt(controlPointIndex)[1];
					}
					else if (leUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int id = leUV->GetIndexArray().GetAt(controlPointIndex);
						uv.x = (float)leUV->GetDirectArray().GetAt(id)[0];
						uv.y = (float)leUV->GetDirectArray().GetAt(id)[1];
					}
				}
				else if (leUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					if (leUV->GetReferenceMode() == FbxGeometryElement::eDirect ||
						leUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int32 textureUVIndex = pMesh->GetTextureUVIndex(vertexID / 3, vertexID % 3);

						uv.x = (float)leUV->GetDirectArray().GetAt(textureUVIndex)[0];
						uv.y = (float)leUV->GetDirectArray().GetAt(textureUVIndex)[1];
					}
				}

				switch (k)
				{
				case 0:
					pStaticMesh->pUV0s[vertexID].x = uv.x;
					pStaticMesh->pUV0s[vertexID].y = uv.y;
					break;
				case 1:
					pStaticMesh->pUV1s[vertexID].x = uv.x;
					pStaticMesh->pUV1s[vertexID].y = uv.y;
					break;
				case 2:
					pStaticMesh->pUV2s[vertexID].x = uv.x;
					pStaticMesh->pUV2s[vertexID].y = uv.y;
					break;
				case 3:
					pStaticMesh->pUV3s[vertexID].x = uv.x;
					pStaticMesh->pUV3s[vertexID].y = uv.y;
					break;
				case 4:
					pStaticMesh->pUV4s[vertexID].x = uv.x;
					pStaticMesh->pUV4s[vertexID].y = uv.y;
					break;
				case 5:
					pStaticMesh->pUV5s[vertexID].x = uv.x;
					pStaticMesh->pUV5s[vertexID].y = uv.y;
					break;
				case 6:
					pStaticMesh->pUV6s[vertexID].x = uv.x;
					pStaticMesh->pUV6s[vertexID].y = uv.y;
					break;
				case 7:
					pStaticMesh->pUV7s[vertexID].x = uv.x;
					pStaticMesh->pUV7s[vertexID].y = uv.y;
					break;
				default:
					break;
				}
			}

			if (normalChannelCount > 0)
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(0);

				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					if (leNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						pStaticMesh->pNormals[vertexID].x = (float)leNormal->GetDirectArray().GetAt(vertexID)[0];
						pStaticMesh->pNormals[vertexID].y = (float)leNormal->GetDirectArray().GetAt(vertexID)[1];
						pStaticMesh->pNormals[vertexID].z = (float)leNormal->GetDirectArray().GetAt(vertexID)[2];
					}
					else if (leNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int id = leNormal->GetIndexArray().GetAt(vertexID);
						pStaticMesh->pNormals[vertexID].x = (float)leNormal->GetDirectArray().GetAt(id)[0];
						pStaticMesh->pNormals[vertexID].y = (float)leNormal->GetDirectArray().GetAt(id)[1];
						pStaticMesh->pNormals[vertexID].z = (float)leNormal->GetDirectArray().GetAt(id)[2];
					}
				}
				else if (leNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					pStaticMesh->pNormals[vertexID].x = (float)leNormal->GetDirectArray().GetAt(controlPointIndex)[0];
					pStaticMesh->pNormals[vertexID].y = (float)leNormal->GetDirectArray().GetAt(controlPointIndex)[1];
					pStaticMesh->pNormals[vertexID].z = (float)leNormal->GetDirectArray().GetAt(controlPointIndex)[2];
				}
			}

			if (tangentChannelCount > 0)
			{
				FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(0);

				if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					if (leTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						pStaticMesh->pTangents[vertexID].x = (float)leTangent->GetDirectArray().GetAt(vertexID)[0];
						pStaticMesh->pTangents[vertexID].y = (float)leTangent->GetDirectArray().GetAt(vertexID)[1];
						pStaticMesh->pTangents[vertexID].z = (float)leTangent->GetDirectArray().GetAt(vertexID)[2];
					}
					else if (leTangent->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int id = leTangent->GetIndexArray().GetAt(vertexID);
						pStaticMesh->pTangents[vertexID].x = (float)leTangent->GetDirectArray().GetAt(id)[0];
						pStaticMesh->pTangents[vertexID].y = (float)leTangent->GetDirectArray().GetAt(id)[1];
						pStaticMesh->pTangents[vertexID].z = (float)leTangent->GetDirectArray().GetAt(id)[2];
					}
				}
			}

			if (binormalChannelCount > 0)
			{
				FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(0);

				if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					if (leBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
					{
						pStaticMesh->pBinormals[vertexID].x = (float)leBinormal->GetDirectArray().GetAt(vertexID)[0];
						pStaticMesh->pBinormals[vertexID].y = (float)leBinormal->GetDirectArray().GetAt(vertexID)[1];
						pStaticMesh->pBinormals[vertexID].z = (float)leBinormal->GetDirectArray().GetAt(vertexID)[2];
					}
					else if (leBinormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					{
						int id = leBinormal->GetIndexArray().GetAt(vertexID);
						pStaticMesh->pBinormals[vertexID].x = (float)leBinormal->GetDirectArray().GetAt(id)[0];
						pStaticMesh->pBinormals[vertexID].y = (float)leBinormal->GetDirectArray().GetAt(id)[1];
						pStaticMesh->pBinormals[vertexID].z = (float)leBinormal->GetDirectArray().GetAt(id)[2];
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////

		pStaticMesh->indexCount = indexCount;

		ctd::map<int32, int32> oldControlPointIndex2NewControlPointIndexMap;

		for (int32 i = 0; i < vertexCount; ++i)
		{
			int32 oldControlPointIndex = pControlPointIndices[i];

			if (oldControlPointIndex2NewControlPointIndexMap.find(oldControlPointIndex) ==
				oldControlPointIndex2NewControlPointIndexMap.end())
			{
				oldControlPointIndex2NewControlPointIndexMap[oldControlPointIndex] = i;
			}
		}
		
		for (int32 i = 0; i < indexCount; ++i)
		{
			pStaticMesh->pIndices[i] = i;
		}

		delete[] pControlPointIndices;

		outStaticMeshes.push_back(std::move(pStaticMesh));

		assert(vertexCount == indexCount);
	}
	
	void FBXImportor::processNode(FbxNode* pNode, ctd::vector<std::unique_ptr<StaticMesh>> & outStaticMeshes, ctd::vector<std::unique_ptr<Material>> & outMaterials, Bool generateTangentsData /*= True*/)
	{
		FbxNodeAttribute::EType lAttributeType;

		if (pNode->GetNodeAttribute() == NULL)
		{
			FBXSDK_printf("NULL Node Attribute\n\n");
		}
		else
		{
			lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

			switch (lAttributeType)
			{
			default:
				break;
			case FbxNodeAttribute::eSkeleton:
				break;
			case FbxNodeAttribute::eMesh:
				processMesh(pNode, outStaticMeshes, outMaterials, generateTangentsData);
				break;
			}
		}

		for (int32 i = 0; i < pNode->GetChildCount(); ++i)
		{
			processNode(pNode->GetChild(i), outStaticMeshes, outMaterials, generateTangentsData);
		}
	}

	ErrorCode FBXImportor::LoadStaticMesh(const ANSICHAR * fileFullPathName, ctd::vector<std::unique_ptr<StaticMesh>> & outStaticMeshes, ctd::vector<std::unique_ptr<Material>> & outMaterials, Bool generateTangentsData /*= True*/)
	{
		FbxManager* pSDKManager = Null;
		FbxScene* pScene = Null;
		ErrorCode result;

		initializeSdkObjects(pSDKManager, pScene);

		result = loadScene(pSDKManager, pScene, fileFullPathName);

		if (result != ErrorCode_OK)
			return result;

		FbxAxisSystem SceneAxisSystem = pScene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);

		if (SceneAxisSystem != OurAxisSystem)
		{
			OurAxisSystem.ConvertScene(pScene);
		}

		// Convert Unit System to what is used in this example, if needed
		FbxSystemUnit SceneSystemUnit = pScene->GetGlobalSettings().GetSystemUnit();

		if (SceneSystemUnit.GetScaleFactor() != 1.0)
		{
			//The unit in this example is centimeter.
			FbxSystemUnit::cm.ConvertScene(pScene);
		}

		FbxGeometryConverter lGeomConverter(pSDKManager);
		lGeomConverter.Triangulate(pScene, true);
		lGeomConverter.SplitMeshesPerMaterial(pScene, true);

		FbxNode * pRootNode = pScene->GetRootNode();

		processNode(pRootNode, outStaticMeshes, outMaterials, generateTangentsData);

		pSDKManager->Destroy();

		return result;
	}
}