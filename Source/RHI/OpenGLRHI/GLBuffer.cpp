#include "GLBuffer.h"

namespace Core
{
	GLBuffer::GLBuffer()
		:m_id(InvalidID)
	{

	}

	uint32 GLBuffer::GetID() const
	{
		return m_id;
	}

	void GLBuffer::Recreate(BufferTarget target, BufferUsage usage, int32 size)
	{
		if (m_id != InvalidID)
		{
			glDeleteBuffers(1, &m_id);
			glCheckError();
		}

		glGenBuffers(1, &m_id);
		glCheckError();

		glBindBuffer(target, m_id);
		glCheckError();

		glBufferData(target, size, Null, usage);
		glCheckError();

		glBindBuffer(target, 0);
		glCheckError();
	}

	void GLBuffer::Activate(BufferTarget target) const
	{
		glBindBuffer(target, m_id);
		glCheckError();
	}

	void GLBuffer::UploadData(BufferTarget target, int32 offset, int32 size, void * pData)
	{
		glBufferSubData(target, offset, size, pData);
		glCheckError();
	}

	void GLBuffer::Inactivate(BufferTarget target) const
	{
		glBindBuffer(target, 0);
		glCheckError();
	}

	void * GLBuffer::Map(BufferTarget target, AccessFlag access)
	{
		void * result = glMapBuffer(target, access);
		glCheckError();

		return result;
	}

	void GLBuffer::Unmap(BufferTarget target)
	{
		glUnmapBuffer(target);
		glCheckError();
	}

	void GLBuffer::ReadPixels(int32 xOffset, int32 yOffset, int32 width, int32 height, GLPixelFormat pixelFormat, GLDataType dataType, void * pOffset)
	{
		glReadPixels(xOffset, yOffset, width, height, pixelFormat, dataType, pOffset);
		glCheckError();
	}

	GLBuffer::~GLBuffer()
	{

	}
}