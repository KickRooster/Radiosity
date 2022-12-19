#include "GLProgram.h"

namespace Core
{
	GLProgram::GLProgram()
	{
		m_id = glCreateProgram();
		glCheckError();
	}

	uint32 GLProgram::GetID() const
	{
		return m_id;
	}

	void GLProgram::AttachShader(GLShader * pShaderAttach)
	{
		glAttachShader(m_id, pShaderAttach->GetID());
		glCheckError();
	}

	void GLProgram::DetachShader(GLShader * pShaderDetach)
	{
		glDetachShader(m_id, pShaderDetach->GetID());
		glCheckError();
	}

	void GLProgram::BindFragDataLocation(uint32 attachIndex, const ANSICHAR * pName)
	{
		glBindFragDataLocation(m_id, attachIndex, pName);
		glCheckError();
	}

	void GLProgram::Link()
	{
		glLinkProgram(m_id);
		glCheckError();
	}

	void GLProgram::Activate() const
	{
		glUseProgram(m_id);
		glCheckError();
	}

	void GLProgram::ActivateTextureSlot(int32 slotIndex, const ANSICHAR * samplerName) const
	{
		glActiveTexture(GL_TEXTURE0 + slotIndex);
		glCheckError();

		int32 loc = glGetUniformLocation(m_id, samplerName);
		glCheckError();

		glUniform1i(loc, slotIndex);
		glCheckError();
	}

	void GLProgram::ActivateImageSlot(int32 slotIndex, const ANSICHAR* ImageName) const
	{
		int32 loc = glGetUniformLocation(m_id, ImageName);
		glCheckError();

		glUniform1i(loc, slotIndex);
		glCheckError();
	}

	void GLProgram::Set4f(const ANSICHAR * pName, float color[4])
	{
		int32 loc = glGetUniformLocation(m_id, pName);
		glCheckError();

		glUniform4f(loc, color[0], color[1], color[2], color[3]);
		glCheckError();
	}

	void GLProgram::Inactivate() const
	{
		glUseProgram(0);
		glCheckError();
	}

	GLProgram::~GLProgram()
	{
		glDeleteProgram(m_id);
		glCheckError();
	}
}