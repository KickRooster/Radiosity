#include "RLProgram.h"

namespace Core
{
	RLProgram::RLProgram()
	{
		m_rawData = rlCreateProgram();
		rlCheckError();
	}

	RLprogram RLProgram::GetRawData()
	{
		return m_rawData;
	}

	void RLProgram::AttachShader(RLShader * pShaderAttach) const
	{
		rlAttachShader(m_rawData, pShaderAttach->GetRawShader());
		rlCheckError();
	}

	void RLProgram::DetachShader(RLShader * pShaderDetach) const
	{
		rlDetachShader(m_rawData, pShaderDetach->GetRawShader());
		rlCheckError();
	}

	void RLProgram::Link()
	{
		rlLinkProgram(m_rawData);
		rlCheckError();

		int32 linkStatus;
		rlGetProgramiv(m_rawData, RL_LINK_STATUS, &linkStatus);
		rlCheckError();

		if (linkStatus != RL_TRUE)
		{
			const ANSICHAR * pParam;

			rlGetProgramString(m_rawData, RL_LINK_LOG, &pParam);
			rlCheckError();
		}
	}

	void RLProgram::Activate() const
	{
		rlUseProgram(m_rawData);
		rlCheckError();
	}

	void RLProgram::Inactivate() const
	{
		rlUseProgram(0);
		rlCheckError();
	}

	void RLProgram::SetUniform1f(const ANSICHAR * pParamName, float value) const
	{
		int32 index = rlGetUniformLocation(m_rawData, pParamName);
		rlCheckError();

		rlUniform1f(index, value);
		rlCheckError();
	}

	void RLProgram::SetUniform3f(const ANSICHAR * pParamName, const Vector3 * pValue) const
	{
		int32 index = rlGetUniformLocation(m_rawData, pParamName);
		rlCheckError();

		rlUniform3f(index, pValue->x, pValue->y, pValue->z);
		rlCheckError();
	}

	void RLProgram::SetUniformMatrix4f(const ANSICHAR * pParamName, const float * pValue) const
	{
		int32 index = rlGetUniformLocation(m_rawData, pParamName);
		rlCheckError();

		rlUniformMatrix4fv(index, 1, RL_FALSE, pValue);
		rlCheckError();
	}

	void RLProgram::SetUniformPrimitive(const ANSICHAR * pParamName, RLprimitive value) const
	{
		RLint index = rlGetUniformLocation(m_rawData, pParamName);
		rlCheckError();

		if (index != -1)
		{
			rlUniformp(index, value);
			rlCheckError();
		}
	}

	void RLProgram::SetTexture(const ANSICHAR * pParamName, RLtexture texture) const
	{
		RLint index = rlGetUniformLocation(m_rawData, pParamName);
		rlCheckError();

		if (index != -1)
		{
			rlUniformt(index, texture);
			rlCheckError();
		}
	}

	RLProgram::~RLProgram()
	{
		rlDeleteProgram(m_rawData);
		rlCheckError();
	}
}