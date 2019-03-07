#pragma once
#include "BaseRL.h"

namespace Core
{
	class RLShader
	{
	private:
		RLShaderType m_type;
		RLshader m_rawData;
		RLShaderStatus m_status;
		std::unique_ptr<ctd::string> m_errorInfo;

	public:
		RLShader(RLShaderType type);
		RLshader GetRawShader();
		ErrorCode Compile(const ANSICHAR * pShaderSource);
		RLShaderStatus GetStatus() const;
		const ANSICHAR * GetErrorInfo() const;
		~RLShader();
	};
}