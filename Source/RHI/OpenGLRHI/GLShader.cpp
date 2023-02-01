#include "GLShader.h"

namespace Core
{
	GLShader::GLShader(GLShaderType type)
		:m_type(type),
		m_status(GLShaderStatus_Undefined)
	{
		m_id = glCreateShader(m_type);
		glCheckError();
	}

	ErrorCode GLShader::Compile(const ANSICHAR * pShaderSource)
	{
		glShaderSource(m_id, 1, &pShaderSource, nullptr);
		glCheckError();

		glCompileShader(m_id);
		glCheckError();

		GLchar * infoBuffer = new GLchar[500];
		GLsizei acturalLength;

		glGetShaderInfoLog(m_id, 500, &acturalLength, infoBuffer);
		glCheckError();

		if (acturalLength != 0)
		{
			m_status = GLShaderStatus_CompileFailed;

			m_errorInfo = std::make_unique<std::string>(infoBuffer);

			return !ErrorCode_OK;
		}

		m_status = GLShaderStatus_OK;

		return ErrorCode_OK;
	}

	uint32 GLShader::GetID() const
	{
		return m_id;
	}

	GLShaderStatus GLShader::GetStatus() const
	{
		return m_status;
	}

	const ANSICHAR * GLShader::GetErrorInfo() const
	{
		return m_errorInfo->c_str();
	}

	GLShader::~GLShader()
	{
		glDeleteShader(m_id);
		glCheckError();
	}
}