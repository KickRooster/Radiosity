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
		m_uv0Size(0),
		m_uv1Size(0),
		m_uv2Size(0),
		m_uv3Size(0),
		m_uv4Size(0),
		m_uv5Size(0),
		m_uv6Size(0),
		m_uv7Size(0),
		m_indexCount(0),
		m_lightmapUVRotated(False),
		pPositions(Null),
		pNormals(Null),
		pTangents(Null),
		pBinormals(Null),
		pColors(Null),
		pUV0s(Null),
		pUV1s(Null),
		pUV2s(Null),
		pUV3s(Null),
		pUV4s(Null),
		pUV5s(Null),
		pUV6s(Null),
		pUV7s(Null),
		pIndices(Null),
		pFirstColocals(Null)
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

	Core::int32 StaticMesh::GetVertexRawDataSize() const
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

	Core::int32 StaticMesh::GetLightmapAtlasWidth() const
	{
		return m_atlasWidth;
	}

	Core::int32 StaticMesh::GetLightmapAtlasHeight() const
	{
		return m_atlasHeight;
	}

	void StaticMesh::RotateUV1HalfPi()
	{
		if (!m_lightmapUVRotated)
		{
			for (int32 i = 0; i < vertexCount; ++i)
			{
				float temp = pUV1s[i].x;

				pUV1s[i].x = pUV1s[i].y;
				pUV1s[i].y = temp;
			}

			//	XXX:	��ͷ���Կ����ܲ��ܵ�������bufferһ����.
			uploadToGPU();

			m_lightmapUVRotated = True;
		}
	}

	void StaticMesh::SetControlPointCount(int32 controlPointCount)
	{
		m_controlPointCount = controlPointCount;
	}

	void StaticMesh::SetLocal2World(Matrix4x4 local2World)
	{
		m_local2World = local2World;
	}

	Core::Matrix4x4 StaticMesh::GetLocal2World() const
	{
		return m_local2World;
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

		if (pFirstColocals)
			delete[] pFirstColocals;
	}
}