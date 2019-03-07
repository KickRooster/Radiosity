#include "RLRenderableUnit.h"

namespace Core
{
	RLRenderableUnit::RLRenderableUnit()
		:
		m_primitive(RL_NULL_PRIMITIVE),
		m_indexBuffer(RL_NULL_BUFFER),
		m_vertexBuffer(RL_NULL_BUFFER),
		m_program(RL_NULL_PROGRAM)
	{
		rlGenPrimitives(1, &m_primitive);
		rlCheckError();
	}

	RLprimitive RLRenderableUnit::GetRawPrimitive() const
	{
		return m_primitive;
	}

	void RLRenderableUnit::Commit(int32 objectID)
	{
		if (!staticMesh.lock()->pIndices ||
			staticMesh.lock()->indexCount == 0 ||
			staticMesh.lock()->vertexCount == 0)
		{
			return ;
		}

		m_program = rlCreateProgram();
		rlCheckError();

		material.lock()->BeginUse();

		rlAttachShader(m_program, material.lock()->rlVertexShader.lock()->GetRLShader()->GetRawShader());
		rlCheckError();

		rlAttachShader(m_program, material.lock()->rlRayShader.lock()->GetRLShader()->GetRawShader());
		rlCheckError();

		rlLinkProgram(m_program);
		rlCheckError();

		int32 linkStatus;
		rlGetProgramiv(m_program, RL_LINK_STATUS, &linkStatus);
		rlCheckError();

		if (linkStatus != RL_TRUE)
		{
			const ANSICHAR * pParam;

			rlGetProgramString(m_program, RL_LINK_LOG, &pParam);
			rlCheckError();
		}

		rlGenBuffers(1, &m_vertexBuffer);
		rlCheckError();

		rlBindBuffer(RL_ARRAY_BUFFER, m_vertexBuffer);
		rlCheckError();

		rlBufferData(RL_ARRAY_BUFFER, staticMesh.lock()->GetVertexRawDataSize(), staticMesh.lock()->GetVertexRawData(), RL_STATIC_DRAW);
		rlCheckError();

		rlBindBuffer(RL_ARRAY_BUFFER, 0);
		rlCheckError();

		rlGenBuffers(1, &m_indexBuffer);
		rlCheckError();

		rlBindBuffer(RL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
		rlCheckError();

		rlBufferData(RL_ELEMENT_ARRAY_BUFFER, sizeof(staticMesh.lock()->pIndices[0]) * staticMesh.lock()->indexCount, staticMesh.lock()->pIndices, RL_STATIC_DRAW);
		rlCheckError();

		rlBindBuffer(RL_ELEMENT_ARRAY_BUFFER, 0);
		rlCheckError();
		
		//////////////////////////////////////////////////////////////////////////

		rlBindPrimitive(RL_PRIMITIVE, m_primitive);
		rlCheckError();

		rlBindBuffer(RL_ARRAY_BUFFER, m_vertexBuffer);
		rlCheckError();

		int32 pointerOffset = 0;

		if (staticMesh.lock()->GetPositionSize() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->positionSlotIndex,
				GetComponentCount(staticMesh.lock()->pPositions[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pPositions[0]);
		}

		if (staticMesh.lock()->GetNormalSize() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->normalSlotIndex,
				GetComponentCount(staticMesh.lock()->pNormals[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pNormals[0]);
		}

		if (staticMesh.lock()->GetTangentSize() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->tangentSlotIndex,
				GetComponentCount(staticMesh.lock()->pTangents[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pTangents[0]);
		}

		if (staticMesh.lock()->GetBinormalSize() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->binormalSlotIndex,
				GetComponentCount(staticMesh.lock()->pBinormals[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pBinormals[0]);
		}

		if (staticMesh.lock()->GetColorSize() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->colorSlotIndex,
				GetComponentCount(staticMesh.lock()->pColors[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pColors[0]);
		}

		if (staticMesh.lock()->GetUV0Size() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->uv0SlotIndex,
				GetComponentCount(staticMesh.lock()->pUV0s[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pUV0s[0]);
		}

		if (staticMesh.lock()->GetUV1Size() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->uv1SlotIndex,
				GetComponentCount(staticMesh.lock()->pUV1s[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pUV1s[0]);
		}

		if (staticMesh.lock()->GetUV2Size() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->uv2SlotIndex,
				GetComponentCount(staticMesh.lock()->pUV2s[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pUV2s[0]);
		}

		if (staticMesh.lock()->GetUV3Size() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->uv3SlotIndex,
				GetComponentCount(staticMesh.lock()->pUV3s[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pUV3s[0]);
		}

		if (staticMesh.lock()->GetUV4Size() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->uv4SlotIndex,
				GetComponentCount(staticMesh.lock()->pUV4s[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pUV4s[0]);
		}

		if (staticMesh.lock()->GetUV5Size() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->uv5SlotIndex,
				GetComponentCount(staticMesh.lock()->pUV5s[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pUV5s[0]);
		}

		if (staticMesh.lock()->GetUV6Size() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->uv6SlotIndex,
				GetComponentCount(staticMesh.lock()->pUV6s[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pUV6s[0]);
		}

		if (staticMesh.lock()->GetUV7Size() > 0)
		{
			rlVertexAttribBuffer(
				staticMesh.lock()->uv7SlotIndex,
				GetComponentCount(staticMesh.lock()->pUV7s[0]),
				RLDataType_Float,
				False,
				staticMesh.lock()->GetVertexRawDataStride(),
				pointerOffset);
			rlCheckError();

			pointerOffset += sizeof(staticMesh.lock()->pUV7s[0]);
		}

		rlBindBuffer(RL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
		rlCheckError();

		rlUseProgram(m_program);
		rlCheckError();

		if (!lightingSettingsUniformBuffer.expired())
		{
			int32 loc = rlGetUniformBlockIndex(m_program, lightingSettingsUniformBuffer.lock()->name);
			rlCheckError();

			rlUniformBlockBuffer(loc, lightingSettingsUniformBuffer.lock()->GetRawData());
			rlCheckError();
		}

		if (!giUniformBuffer.expired())
		{
			int32 loc = rlGetUniformBlockIndex(m_program, giUniformBuffer.lock()->name);
			rlCheckError();

			rlUniformBlockBuffer(loc, giUniformBuffer.lock()->GetRawData());
			rlCheckError();
		}

		if (!material.lock()->albedoTexture.expired())
		{
			int32 loc = rlGetUniformLocation(m_program, material.lock()->albedoSamplerName.c_str());
			rlCheckError();

			rlUniformt(loc, material.lock()->albedoTexture.lock()->GetRLTexture()->GetRawTexture());
			rlCheckError();
		}

		if (!material.lock()->normalMap.expired())
		{
			int32 loc = rlGetUniformLocation(m_program, material.lock()->normalSamplerName.c_str());
			rlCheckError();

			rlUniformt(loc, material.lock()->normalMap.lock()->GetRLTexture()->GetRawTexture());
			rlCheckError();
		}

		{
			int32 loc = rlGetUniformLocation(m_program, material.lock()->idName.c_str());
			rlCheckError();

			if (loc != -1)
			{
				rlUniform1f(loc, objectID / 255.0f);
				rlCheckError();
			}
		}

		{
			int32 loc = rlGetUniformLocation(m_program, material.lock()->albedoColorName.c_str());
			rlCheckError();

			if (loc != -1)
			{
				rlUniform4f(loc, material.lock()->albedoColor[0], material.lock()->albedoColor[1], material.lock()->albedoColor[2], material.lock()->albedoColor[3]);
				rlCheckError();
			}
		}

		rlPrimitiveParameter1i(RL_PRIMITIVE, RL_PRIMITIVE_IS_OCCLUDER, material.lock()->IsOccluder ? RL_TRUE : RL_FALSE);
		rlCheckError();

		rlFrontFace(RL_CCW);
		rlCheckError();

		rlDrawElements(RL_TRIANGLES, staticMesh.lock()->indexCount, RL_UNSIGNED_INT, 0);
		rlCheckError();

		RLenum result = rlCheckPrimitiveStatus(RL_PRIMITIVE);
		rlCheckError();

		assert(result == RL_PRIMITIVE_COMPLETE);

		rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
		rlCheckError();
	}

	void RLRenderableUnit::Activate()
	{
		rlBindPrimitive(RL_PRIMITIVE, m_primitive);
		rlCheckError();
	}

	void RLRenderableUnit::SetAlbdeoTexture()
	{
		if (!material.lock()->albedoTexture.expired())
		{
			int32 loc = rlGetUniformLocation(m_program, material.lock()->albedoSamplerName.c_str());
			rlCheckError();

			rlUniformt(loc, material.lock()->albedoTexture.lock()->GetRLTexture()->GetRawTexture());
			rlCheckError();
		}
	}

	void RLRenderableUnit::SetAlbdeoNormalMap()
	{
		if (!material.lock()->normalMap.expired())
		{
			int32 loc = rlGetUniformLocation(m_program, material.lock()->normalSamplerName.c_str());
			rlCheckError();

			rlUniformt(loc, material.lock()->normalMap.lock()->GetRLTexture()->GetRawTexture());
			rlCheckError();
		}
	}

	void RLRenderableUnit::SetAlbedoColor()
	{
		int32 loc = rlGetUniformLocation(m_program, material.lock()->albedoColorName.c_str());
		rlCheckError();

		if (loc != -1)
		{
			rlUniform4f(loc, material.lock()->albedoColor[0], material.lock()->albedoColor[1], material.lock()->albedoColor[2], material.lock()->albedoColor[3]);
			rlCheckError();
		}
	}

	void RLRenderableUnit::Inactivate()
	{
		rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
		rlCheckError();
	}

	void RLRenderableUnit::UpdateAhead()
	{
		rlDetachShader(m_program, material.lock()->rlVertexShader.lock()->GetRLShader()->GetRawShader());
		rlCheckError();
		
		rlDetachShader(m_program, material.lock()->rlRayShader.lock()->GetRLShader()->GetRawShader());
		rlCheckError();
	}

	void RLRenderableUnit::UpdateBehind()
	{
		rlAttachShader(m_program, material.lock()->rlVertexShader.lock()->GetRLShader()->GetRawShader());
		rlCheckError();
		
		rlAttachShader(m_program, material.lock()->rlRayShader.lock()->GetRLShader()->GetRawShader());
		rlCheckError();

		rlLinkProgram(m_program);
		rlCheckError();

		int32 linkStatus;
		rlGetProgramiv(m_program, RL_LINK_STATUS, &linkStatus);
		rlCheckError();
		
		if (linkStatus != RL_TRUE)
		{
			const ANSICHAR * pParam;

			rlGetProgramString(m_program, RL_LINK_LOG, &pParam);
			rlCheckError();
		}

		if (!lightingSettingsUniformBuffer.expired())
		{
			rlBindPrimitive(RL_PRIMITIVE, m_primitive);
			rlCheckError();

			int32 loc = rlGetUniformBlockIndex(m_program, lightingSettingsUniformBuffer.lock()->name);
			rlCheckError();
		
			rlUniformBlockBuffer(loc, lightingSettingsUniformBuffer.lock()->GetRawData());
			rlCheckError();
		}

		if (!giUniformBuffer.expired())
		{
			rlBindPrimitive(RL_PRIMITIVE, m_primitive);
			rlCheckError();

			int32 loc = rlGetUniformBlockIndex(m_program, giUniformBuffer.lock()->name);
			rlCheckError();

			rlUniformBlockBuffer(loc, giUniformBuffer.lock()->GetRawData());
			rlCheckError();
		}

		if (!material.lock()->albedoTexture.expired())
		{
			rlBindPrimitive(RL_PRIMITIVE, m_primitive);
			rlCheckError();

			int32 loc = rlGetUniformLocation(m_program, material.lock()->albedoSamplerName.c_str());
			rlCheckError();

			rlUniformt(loc, material.lock()->albedoTexture.lock()->GetRLTexture()->GetRawTexture());
			rlCheckError();

			rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
			rlCheckError();
		}

		if (!material.lock()->normalMap.expired())
		{
			rlBindPrimitive(RL_PRIMITIVE, m_primitive);
			rlCheckError();

			int32 loc = rlGetUniformLocation(m_program, material.lock()->normalSamplerName.c_str());
			rlCheckError();

			rlUniformt(loc, material.lock()->normalMap.lock()->GetRLTexture()->GetRawTexture());
			rlCheckError();

			rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
			rlCheckError();
		}
	}

	RLRenderableUnit::~RLRenderableUnit()
	{
		if (m_primitive != RL_NULL_PRIMITIVE)
		{
			rlDeletePrimitives(1, &m_primitive);
			rlCheckError();
		}

		if (m_indexBuffer != RL_NULL_BUFFER)
		{
			rlDeleteBuffers(1, &m_indexBuffer);
			rlCheckError();
		}

		if (m_vertexBuffer != RL_NULL_BUFFER)
		{
			rlDeleteBuffers(1, &m_vertexBuffer);
			rlCheckError();
		}

		if (m_program != RL_NULL_PROGRAM)
		{
			rlDeleteProgram(m_program);
			rlCheckError();
		}
	}
}