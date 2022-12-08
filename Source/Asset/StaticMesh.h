#pragma once
#include <Vector4.h>
#include <Vector3.h>
#include <Vector2.h>
#include "..\RHI\OpenGLRHI\GLBuffer.h"
#include "..\RHI\OpenGLRHI\GLVertexBufferLayout.h"
#include "IAsset.h"
#include "..\Math\Matrix4x4.h"

#include MAP_INCLUDE_PATH
#include VECTOR_INCLUDE_PATH
#include LIST_INCLUDE_PATH
#include SET_INCLUDE_PATH

namespace Core
{
	class StaticMesh : public IAsset
	{
	public:
		const ErrorCode ErrorCode_InvalidData = 0;

	private:
		Matrix4x4 m_local2World;

		float * m_pVertexRawData;

		std::unique_ptr<GLBuffer> m_pVertexBuffer;
		std::unique_ptr<GLBuffer> m_pIndexBuffer; 
		std::unique_ptr<GLVertexBufferLayout> m_pVertexBufferLayout;
		
		int32 m_vertexRawDataSize;
		int32 m_vertexRawDataStride;

		int32 m_positionSize;
		int32 m_normalSize;
		int32 m_tangentSize;
		int32 m_binormalSize;
		int32 m_colorSize;
		int32 m_uv0Size;
		int32 m_uv1Size;
		int32 m_uv2Size;
		int32 m_uv3Size;
		int32 m_uv4Size;
		int32 m_uv5Size;
		int32 m_uv6Size;
		int32 m_uv7Size;

		int32 m_indexCount;
		int32 m_controlPointCount;

		float m_totalSurfaceArea;
		float m_totalUVArea;
		int32 m_radiosityTextureWidth;
		int32 m_radiosityTextureHeight;
		float* m_pPrimitiveSurfaceAreas;
		int32* m_pPrimitiveIDs;
		
		float getArea(const Vector3 & v0, const Vector3 & v1, const Vector3 & v2);
		float getArea(const Vector2 & v0, const Vector2 & v1, const Vector2 & v2);
		float getTriangleArea(const Vector3 & v0, const Vector3 & v1, const Vector3 & v2);
		float getTriangleArea(const Vector2 & v0, const Vector2 & v1, const Vector2 & v2);
		void prepareDataForBaking(Vector3 Scale);
		ErrorCode uploadToGPU();
		void load();

	public:
		const int32 positionSlotIndex = 0;
		const int32 normalSlotIndex = 1;
		const int32 tangentSlotIndex = 2;
		const int32 binormalSlotIndex = 3;
		const int32 colorSlotIndex = 4;
		const int32 uv0SlotIndex = 5;
		const int32 uv1SlotIndex = 6;
		const int32 uv2SlotIndex = 7;
		const int32 uv3SlotIndex = 8;
		const int32 uv4SlotIndex = 9;
		const int32 uv5SlotIndex = 10;
		const int32 uv6SlotIndex = 11;
		const int32 uv7SlotIndex = 12;

		Vector4 * pPositions;	//	λ��
		Vector3 * pNormals;		//	����
		Vector3 * pTangents;	//	����
		Vector3 * pBinormals;	//	������
		Vector4 * pColors;		//	��ɫ		//	TODO:	Ҫ�ĳ�uint8
		Vector2 * pUV0s;		//	UV0
		Vector2 * pUV1s;		//	UV1
		Vector2 * pUV2s;		//	UV2
		Vector2 * pUV3s;		//	UV3
		Vector2 * pUV4s;		//	UV4
		Vector2 * pUV5s;		//	UV5
		Vector2 * pUV6s;		//	UV6
		Vector2 * pUV7s;		//	UV7

		uint32 * pIndices;		//	����
		
		int32 vertexCount;
		int32 indexCount;

		StaticMesh();
		virtual void BeginUse() override;
		virtual void Reload() override;
		virtual void BeforeSave() override;
		virtual void AfterLoad() override;
		void Activate() const;
		void Inactivate() const;
		float * GetVertexRawData() const;
		int32 GetVertexRawDataSize() const;
		int32 GetVertexRawDataStride() const;
		int32 GetPositionSize() const;
		int32 GetNormalSize() const;
		int32 GetTangentSize() const;
		int32 GetBinormalSize() const;
		int32 GetColorSize() const;
		int32 GetUV0Size() const;
		int32 GetUV1Size() const;
		int32 GetUV2Size() const;
		int32 GetUV3Size() const;
		int32 GetUV4Size() const;
		int32 GetUV5Size() const;
		int32 GetUV6Size() const;
		int32 GetUV7Size() const;

		int32 GetRadiosityTextureWidth()const;
		int32 GetRadiosityTextureHeight() const;

		void SetControlPointCount(int32 controlPointCount);

		void SetLocal2World(Matrix4x4 local2World);
		Matrix4x4 GetLocal2World() const;

		virtual ~StaticMesh();
	};
}