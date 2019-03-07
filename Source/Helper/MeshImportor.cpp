#include "MeshImportor.h"

namespace Core
{
	MeshImportor::MeshImportor()
	{
	}

	ErrorCode MeshImportor::LoadModel(const ANSICHAR * fileFullPathName, StaticMesh * pOutStaticMesh)
	{
		unsigned int ppsteps =
			aiProcess_CalcTangentSpace |
			aiProcess_GenNormals |
			aiProcess_JoinIdenticalVertices |
			aiProcess_Triangulate |
			aiProcess_GenUVCoords |
			aiProcess_SortByPType |
			//
			aiProcess_ImproveCacheLocality |
			aiProcess_FixInfacingNormals |
			aiProcess_FindDegenerates |
			aiProcess_FindInvalidData |
			aiProcess_GenUVCoords |
			aiProcess_TransformUVCoords |
			aiProcess_OptimizeMeshes |
			0;

		const aiScene* scene;
		Assimp::Importer Importer;

		//////////////////////////////////////////////////////////////////////////
		aiPropertyStore* props = aiCreatePropertyStore();
		aiSetImportPropertyInteger(props, AI_CONFIG_IMPORT_TER_MAKE_UVS, 1);
		aiSetImportPropertyFloat(props, AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
		aiSetImportPropertyInteger(props, AI_CONFIG_PP_SBP_REMOVE, false ? aiPrimitiveType_LINE | aiPrimitiveType_POINT : 0);

		aiSetImportPropertyInteger(props, AI_CONFIG_GLOB_MEASURE_TIME, 1);

		// Call ASSIMPs C-API to load the file
		scene = (aiScene*)aiImportFileExWithProperties(fileFullPathName,
			ppsteps,
			NULL,
			props);

		if (scene == Null)
			return !ErrorCode_OK;

		aiReleasePropertyStore(props);

		//////////////////////////////////////////////////////////////////////////

		//	TODO:	先把所有子网格都合并成一个

		Bool hasNormal = False;
		Bool hasTangentsAndBitangents = False;
		Bool hasVertexColors = False;
		Bool hasUV0 = False;
		Bool hasUV1 = False;
		Bool hasUV2 = False;
		Bool hasUV3 = False;
		Bool hasUV4 = False;
		Bool hasUV5 = False;
		Bool hasUV6 = False;
		Bool hasUV7 = False;

		for (uint32 i = 0; i < scene->mNumMeshes; ++i)
		{
			pOutStaticMesh->vertexCount += scene->mMeshes[i]->mNumVertices;

			if (scene->mMeshes[i]->HasNormals())
				hasNormal = True;

			if (scene->mMeshes[i]->HasTangentsAndBitangents())
				hasTangentsAndBitangents = True;
			
			if (scene->mMeshes[i]->HasVertexColors(0))
				hasVertexColors = True ;

			if (scene->mMeshes[i]->HasTextureCoords(0))
				hasUV0 = True;

			if (scene->mMeshes[i]->HasTextureCoords(1))
				hasUV1 = True;

			if (scene->mMeshes[i]->HasTextureCoords(2))
				hasUV2 = True;

			if (scene->mMeshes[i]->HasTextureCoords(3))
				hasUV3 = True;

			if (scene->mMeshes[i]->HasTextureCoords(4))
				hasUV4 = True;

			if (scene->mMeshes[i]->HasTextureCoords(5))
				hasUV5 = True;

			if (scene->mMeshes[i]->HasTextureCoords(6))
				hasUV6 = True;

			if (scene->mMeshes[i]->HasTextureCoords(7))
				hasUV7 = True;
		}

		pOutStaticMesh->pPositions = new Vector4[pOutStaticMesh->vertexCount];
		
		if (hasNormal)
			pOutStaticMesh->pNormals = new Vector3[pOutStaticMesh->vertexCount];
		
		if (hasTangentsAndBitangents)
		{
			pOutStaticMesh->pTangents = new Vector3[pOutStaticMesh->vertexCount];
			pOutStaticMesh->pBinormals = new Vector3[pOutStaticMesh->vertexCount];
		}
		
		if (hasVertexColors)
			pOutStaticMesh->pColors = new Vector4[pOutStaticMesh->vertexCount];
		
		if (hasUV0)
			pOutStaticMesh->pUV0s = new Vector2[pOutStaticMesh->vertexCount];

		if (hasUV1)
			pOutStaticMesh->pUV1s = new Vector2[pOutStaticMesh->vertexCount];

		if (hasUV2)
			pOutStaticMesh->pUV2s = new Vector2[pOutStaticMesh->vertexCount];

		if (hasUV3)
			pOutStaticMesh->pUV3s = new Vector2[pOutStaticMesh->vertexCount];

		if (hasUV4)
			pOutStaticMesh->pUV4s = new Vector2[pOutStaticMesh->vertexCount];

		if (hasUV5)
			pOutStaticMesh->pUV5s = new Vector2[pOutStaticMesh->vertexCount];

		if (hasUV6)
			pOutStaticMesh->pUV6s = new Vector2[pOutStaticMesh->vertexCount];

		if (hasUV7)
			pOutStaticMesh->pUV7s = new Vector2[pOutStaticMesh->vertexCount];

		int32 index = 0;

		for (uint32 i = 0; i < scene->mNumMeshes; ++i)
		{
			for (uint32 j = 0; j < scene->mMeshes[i]->mNumVertices; ++j)
			{
				pOutStaticMesh->pPositions[index].x = scene->mMeshes[i]->mVertices[j].x;
				pOutStaticMesh->pPositions[index].y = scene->mMeshes[i]->mVertices[j].y;
				pOutStaticMesh->pPositions[index].z = scene->mMeshes[i]->mVertices[j].z;
				pOutStaticMesh->pPositions[index].w = 1.0f;

				if (hasNormal)
				{
					pOutStaticMesh->pNormals[index].x = scene->mMeshes[i]->mNormals[j].x;
					pOutStaticMesh->pNormals[index].y = scene->mMeshes[i]->mNormals[j].y;
					pOutStaticMesh->pNormals[index].z = scene->mMeshes[i]->mNormals[j].z;
				}

				if (hasTangentsAndBitangents)
				{
					pOutStaticMesh->pTangents[index].x = scene->mMeshes[i]->mTangents[j].x;
					pOutStaticMesh->pTangents[index].y = scene->mMeshes[i]->mTangents[j].y;
					pOutStaticMesh->pTangents[index].z = scene->mMeshes[i]->mTangents[j].z;

					pOutStaticMesh->pBinormals[index].x = scene->mMeshes[i]->mBitangents[j].x;
					pOutStaticMesh->pBinormals[index].y = scene->mMeshes[i]->mBitangents[j].y;
					pOutStaticMesh->pBinormals[index].z = scene->mMeshes[i]->mBitangents[j].z;
				}

				if (hasVertexColors)
				{
					pOutStaticMesh->pColors[index].r = (scene->mMeshes[i]->HasVertexColors(0)) ? scene->mMeshes[i]->mColors[0][j].r : 1.0f;
					pOutStaticMesh->pColors[index].g = (scene->mMeshes[i]->HasVertexColors(0)) ? scene->mMeshes[i]->mColors[0][j].g : 1.0f;
					pOutStaticMesh->pColors[index].b = (scene->mMeshes[i]->HasVertexColors(0)) ? scene->mMeshes[i]->mColors[0][j].b : 1.0f;
				}

				if (hasUV0)
				{
					pOutStaticMesh->pUV0s[index].x = scene->mMeshes[i]->mTextureCoords[0][j].x;
					pOutStaticMesh->pUV0s[index].y = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}

				if (hasUV1)
				{
					pOutStaticMesh->pUV1s[index].x = scene->mMeshes[i]->mTextureCoords[1][j].x;
					pOutStaticMesh->pUV1s[index].y = scene->mMeshes[i]->mTextureCoords[1][j].y;
				}

				if (hasUV2)
				{
					pOutStaticMesh->pUV2s[index].x = scene->mMeshes[i]->mTextureCoords[2][j].x;
					pOutStaticMesh->pUV2s[index].y = scene->mMeshes[i]->mTextureCoords[2][j].y;
				}

				if (hasUV3)
				{
					pOutStaticMesh->pUV3s[index].x = scene->mMeshes[i]->mTextureCoords[3][j].x;
					pOutStaticMesh->pUV3s[index].y = scene->mMeshes[i]->mTextureCoords[3][j].y;
				}

				if (hasUV4)
				{
					pOutStaticMesh->pUV4s[index].x = scene->mMeshes[i]->mTextureCoords[4][j].x;
					pOutStaticMesh->pUV4s[index].y = scene->mMeshes[i]->mTextureCoords[4][j].y;
				}

				if (hasUV5)
				{
					pOutStaticMesh->pUV5s[index].x = scene->mMeshes[i]->mTextureCoords[5][j].x;
					pOutStaticMesh->pUV5s[index].y = scene->mMeshes[i]->mTextureCoords[5][j].y;
				}

				if (hasUV6)
				{
					pOutStaticMesh->pUV6s[index].x = scene->mMeshes[i]->mTextureCoords[6][j].x;
					pOutStaticMesh->pUV6s[index].y = scene->mMeshes[i]->mTextureCoords[6][j].y;
				}

				if (hasUV7)
				{
					pOutStaticMesh->pUV7s[index].x = scene->mMeshes[i]->mTextureCoords[7][j].x;
					pOutStaticMesh->pUV7s[index].y = scene->mMeshes[i]->mTextureCoords[7][j].y;
				}

				++index;
			}
		}

		int32 triangleCount = 0;

		for (uint32 i = 0; i < scene->mNumMeshes; ++i)
		{
			triangleCount += scene->mMeshes[i]->mNumFaces;
		}

		pOutStaticMesh->indexCount = triangleCount * 3;

		pOutStaticMesh->pIndices = new uint32[pOutStaticMesh->indexCount];

		index = 0;

		for (uint32 i = 0; i < scene->mNumMeshes; ++i)
		{
			for (uint32 j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
			{
				for (uint32 k = 0; k < 3; ++k)
				{
					pOutStaticMesh->pIndices[index] = scene->mMeshes[i]->mFaces[j].mIndices[k];
					++index;
				}
			}
		}

		return ErrorCode_OK;
	}

	MeshImportor::~MeshImportor()
	{
	}
}