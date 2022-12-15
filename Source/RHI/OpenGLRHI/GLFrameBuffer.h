#pragma once
#include "BaseGL.h"
#include "GLTexture.h"

#include VECTOR_INCLUDE_PATH

namespace Core
{
	class GLFrameBuffer
	{
	private:
		uint32 m_fboID;
		uint32 m_rboID;
		uint32 m_width;
		uint32 m_height;

		ctd::vector<GLAttachIndex> m_colorAttaches;

		const GLenum singleAttaches[1] = { GL_COLOR_ATTACHMENT0 };
		const GLenum doubleAttaches[2] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1 };
		const GLenum tripleAttaches[3] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		const GLenum fourfoldAttaches[4] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		
		Bool setted = False;

		Bool frameBufferInvalid() const;

	public:
		GLFrameBuffer();
		uint32 GetWidth() const;
		uint32 GetHeight() const;
		void AttachColor(GLAttachIndex attachIndex, GLTextureTarget Target, GLTexture * pColorTexture);
		void ClearAttaches();
		void Resize(uint32 width, uint32 height);
		void Activate() const;
		void Inactivate() const;
		~GLFrameBuffer();
	};
}