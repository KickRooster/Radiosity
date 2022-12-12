#include "StaticMesh.h"
#include "..\Helper\MeshImportor.h"
#include "..\Math\MathGlobal.h"
#include "../3rdParty/UVAtlas/UVAtlas.h"
#include "..\3rdParty\DirectXMesh\DirectXMesh.h"
#include "..\Settings\LightmappingSetting.h"
#include "../3rdParty/TheklaAtlas/thekla_atlas.h"
#include "..\3rdParty\RectangleBinPack\MaxRectsBinPack.h"
#include "..\Helper\FBXImportor.h"

using namespace DirectX;
using namespace Thekla;
using namespace ctd;

namespace Core
{
	float StaticMesh::getArea(const Vector3 & v0, const Vector3 & v1, const Vector3 & v2)
	{
		Vector3 crossValue = Cross(v1 - v0, v2 - v0);

		return abs(length(crossValue)) * 0.5f;
	}

	float StaticMesh::getArea(const Vector2 & v0, const Vector2 & v1, const Vector2 & v2)
	{
		return abs(Cross(v1 - v0, v2 - v0)) * 0.5f;
	}

	float StaticMesh::getTriangleArea(const Vector3 & v0, const Vector3 & v1, const Vector3 & v2)
	{
		Vector3 crossValue = Cross(v1 - v0, v2 - v0);

		return abs(length(crossValue)) * 0.5f;
	}

	float StaticMesh::getTriangleArea(const Vector2 & v0, const Vector2 & v1, const Vector2 & v2)
	{
		float crossValue = Cross(v0 - v2, v1 - v2);

		return abs(crossValue) * 0.5f;
	}

	void StaticMesh::PrepareCustomDataAndPrimitiveMap(Matrix4x4 Object2World)
	{
		m_totalSurfaceArea = 0;
		m_totalUVArea = 0;

		m_leftMost = 0;
		m_rightMost = 0;
		m_topMost = 0;
		m_bottomMost = 0;
		m_zNear = 0;
		m_zFar = 0;

		float* pPrimitiveSurfaceAreas = new float[vertexCount];
		int32* pPrimitiveIDs = new int32[vertexCount];

		PrimitiveMap.empty();

		Vector3 Scale = Vector3(Object2World[0][0], Object2World[1][1], Object2World[2][2]);
		
		if (pUV1s)
		{
			delete[] pUV1s;
		}
		pUV1s = new Vector2[vertexCount];
		memcpy(pUV1s, pUV0s, sizeof(Vector2) * vertexCount);
		
		for (int32 triangleIndex = 0; triangleIndex < indexCount / 3; ++triangleIndex)
		{
			pPrimitiveIDs[triangleIndex * 3] = triangleIndex;
			pPrimitiveIDs[triangleIndex * 3 + 1] = triangleIndex;
			pPrimitiveIDs[triangleIndex * 3 + 2] = triangleIndex;
			
			Vector4 pos0 = pPositions[triangleIndex * 3 + 0];
			Vector4 pos1 = pPositions[triangleIndex * 3 + 1];
			Vector4 pos2 = pPositions[triangleIndex * 3 + 2];

			Vector4 WorldPos0 = Object2World * pos0;
			Vector4 WorldPos1 = Object2World * pos1;
			Vector4 WorldPos2 = Object2World * pos2;

			Vector3 WorldPos0XYZ = Vector3(WorldPos0.x, WorldPos0.y, WorldPos0.z);
			Vector3 WorldPos1XYZ = Vector3(WorldPos1.x, WorldPos1.y, WorldPos1.z);
			Vector3 WorldPos2XYZ = Vector3(WorldPos2.x, WorldPos2.y, WorldPos2.z);

			m_leftMost = min(m_leftMost, pos0.x * Scale.x);
			m_leftMost = min(m_leftMost, pos1.x * Scale.x);
			m_leftMost = min(m_leftMost, pos2.x * Scale.x);

			m_rightMost = max(m_rightMost, pos0.x * Scale.x);
			m_rightMost = max(m_rightMost, pos1.x * Scale.x);
			m_rightMost = max(m_rightMost, pos2.x * Scale.x);

			m_bottomMost = min(m_bottomMost, pos0.y * Scale.y);
			m_bottomMost = min(m_bottomMost, pos1.y * Scale.y);
			m_bottomMost = min(m_bottomMost, pos2.y * Scale.y);
			
			m_topMost = max(m_topMost, pos0.y * Scale.y);
			m_topMost = max(m_topMost, pos1.y * Scale.y);
			m_topMost = max(m_topMost, pos2.y * Scale.y);

			m_zNear = min(m_zNear, pos0.z * Scale.z);
			m_zNear = min(m_zNear, pos1.z * Scale.z);
			m_zNear = min(m_zNear, pos2.z * Scale.z);

			m_zFar = max(m_zFar, pos0.z * Scale.z);
			m_zFar = max(m_zFar, pos1.z * Scale.z);
			m_zFar = max(m_zFar, pos2.z * Scale.z);
			
			float triangleSurfaceArea = getTriangleArea(WorldPos0XYZ, WorldPos1XYZ, WorldPos2XYZ);

			pPrimitiveSurfaceAreas[triangleIndex * 3] = triangleSurfaceArea;
			pPrimitiveSurfaceAreas[triangleIndex * 3 + 1] = triangleSurfaceArea;
			pPrimitiveSurfaceAreas[triangleIndex * 3 + 2] = triangleSurfaceArea;

			m_totalSurfaceArea += triangleSurfaceArea;

			Vector2 uv0 = pUV1s[triangleIndex * 3 + 0];
			Vector2 uv1 = pUV1s[triangleIndex * 3 + 1];
			Vector2 uv2 = pUV1s[triangleIndex * 3 + 2];

			float uvArea = getTriangleArea(uv0, uv1, uv2);

			m_totalUVArea += uvArea;
			
			Primitive Primitive;
			Primitive.ID = static_cast<float>(triangleIndex);
			Vector3 P0P1 = WorldPos1XYZ - WorldPos0XYZ;
			Vector3 P0P2 = WorldPos2XYZ - WorldPos0XYZ;
			Vector3 PrimitiveNormal = Cross(P0P1, P0P2);
			Primitive.Normal = Normalize(PrimitiveNormal);
			Primitive.SurfaceArea = triangleSurfaceArea;
			Vector3 ZeroBarycentricPosition = WorldPos0XYZ / 3.0f + WorldPos1XYZ / 3.0f + WorldPos2XYZ /3.0f;
			Primitive.ShootPosition.x = ZeroBarycentricPosition.x;
			Primitive.ShootPosition.y = ZeroBarycentricPosition.y;
			Primitive.ShootPosition.z = ZeroBarycentricPosition.z;
			Primitive.ShootPosition.w = 1.0f;
			//int32 TexelCount = ceil(LightmappingSetting::Instance()->TexelsPerUnit * triangleSurfaceArea);

			PrimitiveMap[triangleIndex] = Primitive;
		}

		float UVScale = LightmappingSetting::Instance()->TexelsPerUnit / (m_totalUVArea / m_totalSurfaceArea);
		
		float maxU = 0;
		float maxV = 0;
		
		for (int32 i = 0; i < vertexCount; ++i)
		{
			pUV1s[i].x *= UVScale;
			pUV1s[i].y *= UVScale;

			if (pUV1s[i].x * UVScale > maxU)
				maxU = pUV1s[i].x;

			if (pUV1s[i].y > maxV)
				maxV = pUV1s[i].y;
		}

		m_radiosityTextureWidth = ceil(maxU);
		m_radiosityTextureHeight = ceil(maxV);

		if (pCustomData)
		{
			delete[] pCustomData;
		}

		pCustomData = new Vector4[vertexCount];

		for (int32 i = 0; i < vertexCount; ++i)
		{
			pCustomData[i].x = static_cast<float>(pPrimitiveIDs[i]);
			pCustomData[i].y = pPrimitiveSurfaceAreas[i];
			pCustomData[i].z = 0;
			pCustomData[i].w = 0;
		}

		delete[] pPrimitiveSurfaceAreas;
		delete[] pPrimitiveIDs;
	}
	
	ErrorCode StaticMesh::uploadToGPU()
	{
		if (!pIndices ||
			indexCount == 0 ||
			vertexCount == 0)
		{
			return ErrorCode_InvalidData;
		}

		int32 vertexRawDataSize = 0;
		int32 vertexRawDataStride = 0;

		int32 positionSize = pPositions ? sizeof(pPositions[0]) * vertexCount : 0;
		vertexRawDataSize += positionSize;
		vertexRawDataStride += positionSize / vertexCount;

		int32 normalSize = pNormals ? sizeof(pNormals[0]) * vertexCount : 0;
		vertexRawDataSize += normalSize;
		vertexRawDataStride += normalSize / vertexCount;

		int32 tangentSize = pTangents ? sizeof(pTangents[0]) * vertexCount : 0;
		vertexRawDataSize += tangentSize;
		vertexRawDataStride += tangentSize / vertexCount;

		int32 binormalSize = pBinormals ? sizeof(pBinormals[0]) * vertexCount : 0;
		vertexRawDataSize += binormalSize;
		vertexRawDataStride += binormalSize / vertexCount;

		int32 colorSize = pColors ? sizeof(pColors[0]) * vertexCount : 0;
		vertexRawDataSize += colorSize;
		vertexRawDataStride += colorSize / vertexCount;

		int32 customDataSize = pCustomData ? sizeof(pCustomData[0]) * vertexCount : 0;
		vertexRawDataSize += customDataSize;
		vertexRawDataStride += customDataSize / vertexCount;

		int32 uv0Size = pUV0s ? sizeof(pUV0s[0]) * vertexCount : 0;
		vertexRawDataSize += uv0Size;
		vertexRawDataStride += uv0Size / vertexCount;

		int32 uv1Size = pUV1s ? sizeof(pUV1s[0]) * vertexCount : 0;
		vertexRawDataSize += uv1Size;
		vertexRawDataStride += uv1Size / vertexCount;

		int32 uv2Size = pUV2s ? sizeof(pUV2s[0]) * vertexCount : 0;
		vertexRawDataSize += uv2Size;
		vertexRawDataStride += uv2Size / vertexCount;

		int32 uv3Size = pUV3s ? sizeof(pUV3s[0]) * vertexCount : 0;
		vertexRawDataSize += uv3Size;
		vertexRawDataStride += uv3Size / vertexCount;

		int32 uv4Size = pUV4s ? sizeof(pUV4s[0]) * vertexCount : 0;
		vertexRawDataSize += uv4Size;
		vertexRawDataStride += uv4Size / vertexCount;

		int32 uv5Size = pUV5s ? sizeof(pUV5s[0]) * vertexCount : 0;
		vertexRawDataSize += uv5Size;
		vertexRawDataStride += uv5Size / vertexCount;

		int32 uv6Size = pUV6s ? sizeof(pUV6s[0]) * vertexCount : 0;
		vertexRawDataSize += uv6Size;
		vertexRawDataStride += uv6Size / vertexCount;

		int32 uv7Size = pUV7s ? sizeof(pUV7s[0]) * vertexCount : 0;
		vertexRawDataSize += uv7Size;
		vertexRawDataStride += uv7Size / vertexCount;

		m_vertexRawDataSize = vertexRawDataSize;
		m_vertexRawDataStride = vertexRawDataStride;

		if (positionSize != m_positionSize ||
			normalSize != m_normalSize ||
			tangentSize != m_tangentSize ||
			binormalSize != m_binormalSize ||
			colorSize != m_colorSize ||
			customDataSize != m_customDataSize ||
			uv0Size != m_uv0Size ||
			uv1Size != m_uv1Size ||
			uv2Size != m_uv2Size ||
			uv3Size != m_uv3Size ||
			uv4Size != m_uv4Size ||
			uv5Size != m_uv5Size ||
			uv6Size != m_uv6Size ||
			uv7Size != m_uv7Size)
		{
			if (m_pVertexRawData)
				delete[] m_pVertexRawData;

			m_pVertexRawData = new float[vertexRawDataSize];

			m_pVertexBuffer->Recreate(BufferTarget_ArrayBuffer, BufferUsage_StaticDraw, vertexRawDataSize);
		}

		m_positionSize = positionSize;
		m_normalSize = normalSize;
		m_tangentSize = tangentSize;
		m_binormalSize = binormalSize;
		m_colorSize = colorSize;
		m_customDataSize = customDataSize;
		m_uv0Size = uv0Size;
		m_uv1Size = uv1Size;
		m_uv2Size = uv2Size;
		m_uv3Size = uv3Size;
		m_uv4Size = uv4Size;
		m_uv5Size = uv5Size;
		m_uv6Size = uv6Size;
		m_uv7Size = uv7Size;

		int32 offset = 0;

		for (int32 i = 0; i < vertexCount; ++i)
		{
			if (positionSize > 0)
			{
				memcpy(m_pVertexRawData + offset, pPositions + i, sizeof(pPositions[0]));
				offset += GetComponentCount(pPositions[0]);
			}

			if (normalSize > 0)
			{
				memcpy(m_pVertexRawData + offset, pNormals + i, sizeof(pNormals[0]));
				offset += GetComponentCount(pNormals[0]);
			}

			if (tangentSize > 0)
			{
				memcpy(m_pVertexRawData + offset, pTangents + i, sizeof(pTangents[0]));
				offset += GetComponentCount(pTangents[0]);
			}

			if (binormalSize > 0)
			{
				memcpy(m_pVertexRawData + offset, pBinormals + i, sizeof(pBinormals[0]));
				offset += GetComponentCount(pBinormals[0]);
			}

			if (colorSize > 0)
			{
				memcpy(m_pVertexRawData + offset, pColors + i, sizeof(pColors[0]));
				offset += GetComponentCount(pColors[0]);
			}

			if (customDataSize > 0)
			{
				memcpy(m_pVertexRawData + offset, pCustomData + i, sizeof(pCustomData[0]));
				offset += GetComponentCount(pCustomData[0]);
			}

			if (uv0Size > 0)
			{
				memcpy(m_pVertexRawData + offset, pUV0s + i, sizeof(pUV0s[0]));
				offset += GetComponentCount(pUV0s[0]);
			}

			if (uv1Size > 0)
			{
				memcpy(m_pVertexRawData + offset, pUV1s + i, sizeof(pUV1s[0]));
				offset += GetComponentCount(pUV1s[0]);
			}

			if (uv2Size > 0)
			{
				memcpy(m_pVertexRawData + offset, pUV2s + i, sizeof(pUV2s[0]));
				offset += GetComponentCount(pUV2s[0]);
			}

			if (uv3Size > 0)
			{
				memcpy(m_pVertexRawData + offset, pUV3s + i, sizeof(pUV3s[0]));
				offset += GetComponentCount(pUV3s[0]);
			}

			if (uv4Size > 0)
			{
				memcpy(m_pVertexRawData + offset, pUV4s + i, sizeof(pUV4s[0]));
				offset += GetComponentCount(pUV4s[0]);
			}

			if (uv5Size > 0)
			{
				memcpy(m_pVertexRawData + offset, pUV5s + i, sizeof(pUV5s[0]));
				offset += GetComponentCount(pUV5s[0]);
			}

			if (uv6Size > 0)
			{
				memcpy(m_pVertexRawData + offset, pUV6s + i, sizeof(pUV6s[0]));
				offset += GetComponentCount(pUV6s[0]);
			}

			if (uv7Size > 0)
			{
				memcpy(m_pVertexRawData + offset, pUV7s + i, sizeof(pUV7s[0]));
				offset += GetComponentCount(pUV7s[0]);
			}
		}

		m_pVertexBufferLayout->Activate();
		m_pVertexBuffer->Activate(BufferTarget_ArrayBuffer);
		m_pVertexBuffer->UploadData(BufferTarget_ArrayBuffer, 0, vertexRawDataSize, m_pVertexRawData);

		int64 pointerOffset = 0;

		if (positionSize > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				positionSlotIndex,
				GetComponentCount(pPositions[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));	//	����һ������

			pointerOffset += sizeof(pPositions[0]);
		}

		if (normalSize > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				normalSlotIndex,
				GetComponentCount(pNormals[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pNormals[0]);
		}

		if (tangentSize > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				tangentSlotIndex,
				GetComponentCount(pTangents[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pTangents[0]);
		}

		if (binormalSize > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				binormalSlotIndex,
				GetComponentCount(pBinormals[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pBinormals[0]);
		}

		if (colorSize > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				colorSlotIndex,
				GetComponentCount(pColors[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pColors[0]);
		}

		if (customDataSize > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				customDataSlotIndex,
				GetComponentCount(pCustomData[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pCustomData[0]);
		}

		if (uv0Size > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				uv0SlotIndex,
				GetComponentCount(pUV0s[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pUV0s[0]);
		}

		if (uv1Size > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				uv1SlotIndex,
				GetComponentCount(pUV1s[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pUV1s[0]);
		}

		if (uv2Size > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				uv2SlotIndex,
				GetComponentCount(pUV2s[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pUV2s[0]);
		}

		if (uv3Size > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				uv3SlotIndex,
				GetComponentCount(pUV3s[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pUV3s[0]);
		}

		if (uv4Size > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				uv4SlotIndex,
				GetComponentCount(pUV4s[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pUV4s[0]);
		}

		if (uv5Size > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				uv5SlotIndex,
				GetComponentCount(pUV5s[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pUV5s[0]);
		}

		if (uv6Size > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				uv6SlotIndex,
				GetComponentCount(pUV6s[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pUV6s[0]);
		}

		if (uv7Size > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				uv7SlotIndex,
				GetComponentCount(pUV7s[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pUV7s[0]);
		}

		if (indexCount != m_indexCount)
		{
			m_pIndexBuffer->Recreate(BufferTarget_ElementArray, BufferUsage_StaticDraw, sizeof(m_pVertexRawData[0]) * indexCount);
		}

		m_indexCount = indexCount;

		m_pIndexBuffer->Activate(BufferTarget_ElementArray);
		m_pIndexBuffer->UploadData(BufferTarget_ElementArray, 0, sizeof(m_pVertexRawData[0]) * indexCount, pIndices);

		m_pVertexBufferLayout->Inactivate();

		return ErrorCode_OK;
	}

	void StaticMesh::load()
	{
		uploadToGPU();
	}

	StaticMesh::StaticMesh()
		:m_pVertexRawData(Null),
		m_local2World(Matrix4x4Identify),
		m_pVertexBuffer(std::make_unique<GLBuffer>()),
		m_pIndexBuffer(std::make_unique<GLBuffer>()),
		m_pVertexBufferLayout(std::make_unique<GLVertexBufferLayout>()),
		vertexCount(0),
		m_positionSize(0),
		m_normalSize(0),
		m_tangentSize(0),
		m_binormalSize(0),
		m_colorSize(0),
		m_customDataSize(0),
		m_uv0Size(0),
		m_uv1Size(0),
		m_uv2Size(0),
		m_uv3Size(0),
		m_uv4Size(0),
		m_uv5Size(0),
		m_uv6Size(0),
		m_uv7Size(0),
		m_indexCount(0),
		m_totalSurfaceArea(0),
		m_totalUVArea(0),
		pPositions(Null),
		pNormals(Null),
		pTangents(Null),
		pBinormals(Null),
		pColors(Null),
		pCustomData(Null),
		pUV0s(Null),
		pUV1s(Null),
		pUV2s(Null),
		pUV3s(Null),
		pUV4s(Null),
		pUV5s(Null),
		pUV6s(Null),
		pUV7s(Null),
		pIndices(Null)
	{
	}

	void StaticMesh::BeginUse()
	{
		if (!m_beingUsed)
		{
			load();
			m_beingUsed = True;
		}
	}

	void StaticMesh::Reload()
	{
		load();
	}

	void StaticMesh::BeforeSave()
	{
	}

	void StaticMesh::AfterLoad()
	{
	}

	void StaticMesh::Activate() const
	{
		m_pVertexBufferLayout->Activate();
	}

	void StaticMesh::Inactivate() const
	{
		m_pVertexBufferLayout->Inactivate();
	}

	float * StaticMesh::GetVertexRawData() const
	{
		return m_pVertexRawData;
	}

	int32 StaticMesh::GetVertexRawDataSize() const
	{
		return m_vertexRawDataSize;
	}

	Core::int32 StaticMesh::GetVertexRawDataStride() const
	{
		return m_vertexRawDataStride;
	}

	int32 StaticMesh::GetPositionSize() const
	{
		return m_positionSize;
	}

	int32 StaticMesh::GetNormalSize() const
	{
		return m_normalSize;
	}

	int32 StaticMesh::GetTangentSize() const
	{
		return m_tangentSize;
	}

	int32 StaticMesh::GetBinormalSize() const
	{
		return m_binormalSize;
	}

	int32 StaticMesh::GetColorSize() const
	{
		return m_colorSize;
	}

	int32 StaticMesh::GetUV0Size() const
	{
		return m_uv0Size;
	}

	int32 StaticMesh::GetUV1Size() const
	{
		return m_uv1Size;
	}

	int32 StaticMesh::GetUV2Size() const
	{
		return m_uv2Size;
	}

	int32 StaticMesh::GetUV3Size() const
	{
		return m_uv3Size;
	}

	int32 StaticMesh::GetUV4Size() const
	{
		return m_uv4Size;
	}

	int32 StaticMesh::GetUV5Size() const
	{
		return m_uv5Size;
	}

	int32 StaticMesh::GetUV6Size() const
	{
		return m_uv6Size;
	}

	int32 StaticMesh::GetUV7Size() const
	{
		return m_uv7Size;
	}

	int32 StaticMesh::GetRadiosityTextureWidth() const
	{
		return m_radiosityTextureWidth;
	}

	int32 StaticMesh::GetRadiosityTextureHeight() const
	{
		return m_radiosityTextureHeight;
	}

	float StaticMesh::GetLeftMost()
	{
		return m_leftMost;
	}

	float StaticMesh::GetRightMost()
	{
		return m_rightMost;
	}

	float StaticMesh::GetBottomMost()
	{
		return m_bottomMost;
	}

	float StaticMesh::GetTopMost()
	{
		return m_topMost;
	}

	float StaticMesh::GetZNear()
	{
		return m_zNear;
	}

	float StaticMesh::GetZFar()
	{
		return m_zFar;
	}
	
	void StaticMesh::SetControlPointCount(int32 controlPointCount)
	{
		m_controlPointCount = controlPointCount;
	}

	void StaticMesh::SetLocal2World(Matrix4x4 local2World)
	{
		m_local2World = local2World;
	}

	Matrix4x4 StaticMesh::GetLocal2World() const
	{
		return m_local2World;
	}

	void StaticMesh::BeforeBaking(Matrix4x4 Object2World)
	{
		PrepareCustomDataAndPrimitiveMap(Object2World);
		uploadToGPU();
	}
	
	StaticMesh::~StaticMesh()
	{
		if (pPositions)
			delete[] pPositions;

		if (pNormals)
			delete[] pNormals;

		if (pTangents)
			delete[] pTangents;

		if (pBinormals)
			delete[] pBinormals;

		if (pColors)
			delete[] pColors;

		if (pCustomData)
		{
			delete[] pCustomData;
		}

		if (pUV0s)
			delete[] pUV0s;

		if (pUV1s)
			delete[] pUV1s;

		if (pUV2s)
			delete[] pUV2s;

		if (pUV3s)
			delete[] pUV3s;

		if (pUV4s)
			delete[] pUV4s;

		if (pUV5s)
			delete[] pUV5s;

		if (pUV6s)
			delete[] pUV6s;

		if (pUV7s)
			delete[] pUV7s;

		if (pIndices)
			delete[] pIndices;
	}
}