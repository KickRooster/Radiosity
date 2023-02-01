#pragma once
#include "BaseGL.h"

namespace Core
{
	class GLShader
	{
	private:
		GLShaderType m_type;
		uint32 m_id;
		GLShaderStatus m_status;
		std::unique_ptr<std::string> m_errorInfo;

	public:
		GLShader(GLShaderType type);
		ErrorCode Compile(const ANSICHAR * pShaderSource);
		uint32 GetID() const;
		GLShaderStatus GetStatus() const;
		const ANSICHAR * GetErrorInfo() const;
		~GLShader();
	};
}