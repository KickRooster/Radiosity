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

		float* pPrimitiveSurfaceAreas = new float[vertexCount / 3];
		int32* pPrimitiveIDs = new int32[vertexCount / 3];

		PrimitiveMap.empty();
		
		if (pUV1s)
		{
			delete[] pUV1s;
		}
		pUV1s = new Vector2[vertexCount];
		memcpy(pUV1s, pUV0s, sizeof(Vector2) * vertexCount);
		
		for (int32 triangleIndex = 0; triangleIndex < indexCount / 3; ++triangleIndex)
		{
			pPrimitiveIDs[triangleIndex] = triangleIndex;
			
			Vector4 pos0 = pPositions[triangleIndex * 3 + 0];
			Vector4 pos1 = pPositions[triangleIndex * 3 + 1];
			Vector4 pos2 = pPositions[triangleIndex * 3 + 2];

			Vector4 WorldPos0 = Object2World * pos0;
			Vector4 WorldPos1 = Object2World * pos1;
			Vector4 WorldPos2 = Object2World * pos2;

			Vector3 WorldPos0XYZ = Vector3(WorldPos0.x, WorldPos0.y, WorldPos0.z);
			Vector3 WorldPos1XYZ = Vector3(WorldPos1.x, WorldPos1.y, WorldPos1.z);
			Vector3 WorldPos2XYZ = Vector3(WorldPos2.x, WorldPos2.y, WorldPos2.z);
			
			float triangleSurfaceArea = getTriangleArea(WorldPos0XYZ, WorldPos1XYZ, WorldPos2XYZ);

			pPrimitiveSurfaceAreas[triangleIndex] = triangleSurfaceArea;

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
			Primitive.ZeroBarycentricPosition.x = ZeroBarycentricPosition.x;
			Primitive.ZeroBarycentricPosition.y = ZeroBarycentricPosition.y;
			Primitive.ZeroBarycentricPosition.z = ZeroBarycentricPosition.z;
			Primitive.ZeroBarycentricPosition.w = 1.0f;
			
			PrimitiveMap[triangleIndex] = Primitive;
		}
		
		float UVScale = LightmappingSetting::Instance()->TexelsPerUnit * m_totalSurfaceArea / m_totalUVArea;
		float UVScaleSqrt = sqrt(UVScale);
		
		float maxU = 0;
		float maxV = 0;

		for (int32 i = 0; i < vertexCount; ++i)
		{
			if (pUV1s[i].x > maxU)
				maxU = pUV1s[i].x;

			if (pUV1s[i].y > maxV)
				maxV = pUV1s[i].y;
		}

		float UScale = maxU / maxV * UVScaleSqrt;
		float VScale = maxV / maxU * UVScaleSqrt;
		
		m_radiosityTextureWidth = ceil(maxU * UScale);
		m_radiosityTextureHeight = ceil(maxV * VScale);

		if (pCustomData)
		{
			delete[] pCustomData;
		}

		pCustomData = new Vector4[vertexCount];

		for (int32 i = 0; i < vertexCount; ++i)
		{
			pCustomData[i].x = static_cast<float>(pPrimitiveIDs[i / 3]);
			pCustomData[i].y = pPrimitiveSurfaceAreas[i / 3];
			pCustomData[i].z = 0;
			pCustomData[i].w = 0;
		}

		delete[] pPrimitiveSurfaceAreas;
		delete[] pPrimitiveIDs;

		if (pScaledUV1Positions)
		{
			delete[] pScaledUV1Positions;
		}

		pScaledUV1Positions = new Vector4[vertexCount];

		for (int32 i = 0; i < vertexCount; ++i)
		{
			pScaledUV1Positions[i].x = pUV1s[i].x * UScale;
			pScaledUV1Positions[i].y = pUV1s[i].y * VScale;
			pScaledUV1Positions[i].z = 0;
			pScaledUV1Positions[i].w = 1.0f;
		}

		for (int32 triangleIndex = 0; triangleIndex < indexCount / 3; ++triangleIndex)
		{
			Primitive& Primitive = PrimitiveMap[triangleIndex];
			
			Vector4 pos0 = pScaledUV1Positions[triangleIndex * 3 + 0];
			Vector4 pos1 = pScaledUV1Positions[triangleIndex * 3 + 1];
			Vector4 pos2 = pScaledUV1Positions[triangleIndex * 3 + 2];

			Vector3 ZeroBarycentricPosition = pos0 / 3.0f + pos1 / 3.0f + pos2 / 3.0f;
			Primitive.UV1ZeroBarycentricPosition.x = ZeroBarycentricPosition.x;
			Primitive.UV1ZeroBarycentricPosition.y = ZeroBarycentricPosition.y;
			Primitive.UV1ZeroBarycentricPosition.z = ZeroBarycentricPosition.z;
			Primitive.UV1ZeroBarycentricPosition.w = 1.0f;
			
			Vector3 P0P1 = pos1 - pos0;
			Vector3 P0P2 = pos2 - pos0;
			Vector3 PrimitiveNormal = Cross(P0P1, P0P2);
			Primitive.UV1Normal = Normalize(PrimitiveNormal);

			Vector3 Pos03 = Vector3(pos0.x, pos0.y, pos0.z);
			Vector3 Pos13 = Vector3(pos1.x, pos1.y, pos1.z);
			Vector3 Pos23 = Vector3(pos2.x, pos2.y, pos2.z);
			Primitive.ScaledUV1Area = getTriangleArea(Pos03, Pos13, Pos23);
		}
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

		int32 scaledUV1PositionSize = pScaledUV1Positions ? sizeof(pScaledUV1Positions[0]) * vertexCount : 0;
		vertexRawDataSize += scaledUV1PositionSize;
		vertexRawDataStride += scaledUV1PositionSize / vertexCount;

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

			if (scaledUV1PositionSize > 0)
			{
				memcpy(m_pVertexRawData + offset, pScaledUV1Positions + i, sizeof(pScaledUV1Positions[0]));
				offset += GetComponentCount(pScaledUV1Positions[0]);
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

		if (scaledUV1PositionSize > 0)
		{
			m_pVertexBufferLayout->SetSlotElement(
				scaledUV1PositionIndex,
				GetComponentCount(pScaledUV1Positions[0]),
				GLDataType_Float,
				False,
				vertexRawDataStride,
				reinterpret_cast<GLvoid *>(pointerOffset));

			pointerOffset += sizeof(pScaledUV1Positions[0]);
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
		pScaledUV1Positions(Null),
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

	float StaticMesh::GetTotalSurfaceArea() const
	{
		return m_totalSurfaceArea;
	}

	float StaticMesh::GetTotalUVArea() const
	{
		return m_totalUVArea;
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

	void StaticMesh::CalculateOrthoParameters(int32 StartPrimitive, int32 PrimitiveCount, const Matrix4x4& View, float& OutLeftMost, float& OutRightMost, float& OutBottomMost, float& OutTopMost, float& OutZNear, float& OutZFar)
	{
		OutLeftMost = 0;
		OutRightMost = 0;
		OutBottomMost = 0;
		OutTopMost = 0;
		OutZNear = 0;
		OutZFar = 0;
		
		for (int32 PrimitiveIndex = StartPrimitive; PrimitiveIndex < StartPrimitive + PrimitiveCount; ++PrimitiveIndex)
		{
			Vector4 WorldPos0 = pScaledUV1Positions[PrimitiveIndex * 3 + 0];
			Vector4 WorldPos1 = pScaledUV1Positions[PrimitiveIndex * 3 + 1];
			Vector4 WorldPos2 = pScaledUV1Positions[PrimitiveIndex * 3 + 2];

			Vector4 ViewPos0 = View * WorldPos0;
			Vector4 ViewPos1 = View * WorldPos1;
			Vector4 ViewPos2 = View * WorldPos2;

			OutLeftMost = min(OutLeftMost, ViewPos0.x);
			OutLeftMost = min(OutLeftMost, ViewPos1.x);
			OutLeftMost = min(OutLeftMost, ViewPos2.x);

			OutRightMost = max(OutRightMost, ViewPos0.x);
			OutRightMost = max(OutRightMost, ViewPos1.x);
			OutRightMost = max(OutRightMost, ViewPos2.x);

			OutBottomMost = min(OutBottomMost, ViewPos0.y);
			OutBottomMost = min(OutBottomMost, ViewPos1.y);
			OutBottomMost = min(OutBottomMost, ViewPos2.y);
			
			OutTopMost = max(OutTopMost, ViewPos0.y);
			OutTopMost = max(OutTopMost, ViewPos1.y);
			OutTopMost = max(OutTopMost, ViewPos2.y);

			OutZNear = min(OutZNear, ViewPos0.z);
			OutZNear = min(OutZNear, ViewPos1.z);
			OutZNear = min(OutZNear, ViewPos2.z);

			OutZFar = max(OutZFar, ViewPos0.z);
			OutZFar = max(OutZFar, ViewPos1.z);
			OutZFar = max(OutZFar, ViewPos2.z);
		}
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

		if (pScaledUV1Positions)
		{
			delete[] pScaledUV1Positions;
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