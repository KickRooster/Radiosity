#include "GLVertexBufferLayout.h"

namespace Core
{
	GLVertexBufferLayout::GLVertexBufferLayout()
	{
		glGenVertexArrays(1, &m_vaoID);
		glCheckError();
	}

	void GLVertexBufferLayout::Activate() const
	{
		glBindVertexArray(m_vaoID);
		glCheckError();
	}

	void GLVertexBufferLayout::SetSlotElement(uint32 index, int32 size, GLDataType dataType, Bool normalized, int32 stride, const GLvoid * pointer) const
	{
		glEnableVertexAttribArray(index);
		glCheckError();

		glVertexAttribPointer(index, size, dataType, normalized, stride, pointer);
		glCheckError();
	}

	void GLVertexBufferLayout::Inactivate() const
	{
		glBindVertexArray(0);
		glCheckError();
	}

	GLVertexBufferLayout::~GLVertexBufferLayout()
	{
		glDeleteVertexArrays(1, &m_vaoID);
		glCheckError();
	}
}