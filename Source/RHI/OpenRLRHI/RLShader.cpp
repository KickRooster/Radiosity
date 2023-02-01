#include "RLShader.h"

namespace Core
{
	RLShader::RLShader(RLShaderType type)
		:m_status(RLShaderStatus_Undefined),
		m_type(type)
	{
		m_rawData = rlCreateShader(m_type);
		rlCheckError();
	}

	RLshader RLShader::GetRawShader()
	{
		return m_rawData;
	}

	ErrorCode RLShader::Compile(const ANSICHAR * pShaderSource)
	{
		rlShaderSource(m_rawData, 1, &pShaderSource, NULL);
		rlCompileShader(m_rawData);

		int32 compileStatus;
		rlGetShaderiv(m_rawData, RL_COMPILE_STATUS, &compileStatus);

		if (compileStatus != RL_TRUE)
		{
			m_status = RLShaderStatus_CompileFailed;

			const ANSICHAR * pParam;

			rlGetShaderString(m_rawData, RL_COMPILE_LOG, &pParam);

			m_errorInfo = std::make_unique<std::string>(pParam);

			return !ErrorCode_OK;
		}

		m_status = RLShaderStatus_OK;

		return ErrorCode_OK;
	}

	RLShaderStatus RLShader::GetStatus() const
	{
		return m_status;
	}

	const ANSICHAR * RLShader::GetErrorInfo() const
	{
		return m_errorInfo->c_str();
	}

	RLShader::~RLShader()
	{
		rlDeleteShader(m_rawData);
		rlCheckError();
	}
}