#include "GLFrameBuffer.h"

namespace Core
{
	Bool GLFrameBuffer::frameBufferInvalid() const
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		glCheckError();

		switch (status)
		{
		case GL_FRAMEBUFFER_COMPLETE:
			status = GL_FRAMEBUFFER_COMPLETE;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			status = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			status = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			status = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			status = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER;
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			status = GL_FRAMEBUFFER_UNSUPPORTED_EXT;
			break;
		default:
			break;
		}

		return status == GL_FRAMEBUFFER_COMPLETE;
	}

	GLFrameBuffer::GLFrameBuffer()
		:m_fboID(InvalidID),
		m_rboID(InvalidID),
		m_width(0),
		m_height(0)
	{

	}

	Core::uint32 GLFrameBuffer::GetWidth() const
	{
		return m_width;
	}

	Core::uint32 GLFrameBuffer::GetHeight() const
	{
		return m_height;
	}

	void GLFrameBuffer::AttachColor(GLAttachIndex attachIndex, GLTextureTarget Target, GLTexture * pColorTexture)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
		glCheckError();

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachIndex, Target, pColorTexture->GetID(), 0);
		glCheckError();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCheckError();

		m_colorAttaches.push_back(attachIndex);
	}

	void GLFrameBuffer::ClearAttaches()
	{
		m_colorAttaches.empty();
	}
	
	void GLFrameBuffer::Resize(uint32 width, uint32 height)
	{
		m_width = width;
		m_height = height;

		if (m_fboID != InvalidID)
			glDeleteFramebuffers(1, &m_fboID);

		glGenFramebuffers(1, &m_fboID);
		glCheckError();

		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
		glCheckError();

		if (m_rboID != InvalidID)
		{
			glDeleteRenderbuffers(1, &m_rboID);
			glCheckError();
		}

		glGenRenderbuffers(1, &m_rboID);
		glCheckError();

		glBindRenderbuffer(GL_RENDERBUFFER, m_rboID);
		glCheckError();

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glCheckError();

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glCheckError();

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboID);
		glCheckError();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCheckError();
	}

	void GLFrameBuffer::Activate() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
		glCheckError();

		if (m_colorAttaches.size() == 1)
		{
			glDrawBuffers((GLsizei)m_colorAttaches.size(), singleAttaches);
			glCheckError();
		}
		else if (m_colorAttaches.size() == 2)
		{
			glDrawBuffers((GLsizei)m_colorAttaches.size(), doubleAttaches);
			glCheckError();
		}
		else if (m_colorAttaches.size() == 3)
		{
			glDrawBuffers((GLsizei)m_colorAttaches.size(), tripleAttaches);
			glCheckError();
		}
		else if (m_colorAttaches.size() == 4)
		{
			glDrawBuffers((GLsizei)m_colorAttaches.size(), fourfoldAttaches);
			glCheckError();
		}

		frameBufferInvalid();
	}

	void GLFrameBuffer::Inactivate() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCheckError();
	}

	GLFrameBuffer::~GLFrameBuffer()
	{

	}
}