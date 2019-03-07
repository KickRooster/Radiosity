#include "RLFrameBuffer.h"

namespace Core
{
	RLFrameBuffer::RLFrameBuffer()
	{
		rlGenFramebuffers(1, &m_rawData);
		rlCheckError();
	}

	void RLFrameBuffer::AttachColor(RLAttachIndex attachIndex, RLTexture2D * pColorTexture)
	{
		rlBindFramebuffer(RL_FRAMEBUFFER, m_rawData);
		rlCheckError();

		rlFramebufferTexture2D(RL_FRAMEBUFFER, attachIndex, RL_TEXTURE_2D, pColorTexture->GetRawTexture(), 0);
		rlCheckError(); 
		
		rlBindFramebuffer(RL_FRAMEBUFFER, 0);
		rlCheckError();
	}

	void RLFrameBuffer::Resize(uint32 width, uint32 height)
	{
		m_width = width;
		m_height = height;

		if (m_rawData)
		{
			rlDeleteFramebuffers(1, &m_rawData);
			rlCheckError();
		}

		rlGenFramebuffers(1, &m_rawData);
		rlCheckError();
	}

	Core::uint32 RLFrameBuffer::GetWidth() const
	{
		return m_width;
	}

	Core::uint32 RLFrameBuffer::GetHeight() const
	{
		return m_height;
	}

	void RLFrameBuffer::Activate() const
	{
		rlBindFramebuffer(RL_FRAMEBUFFER, m_rawData);
		rlCheckError();
	}

	void RLFrameBuffer::Inactivate() const
	{
		rlBindFramebuffer(RL_FRAMEBUFFER, 0);
		rlCheckError();
	}

	RLFrameBuffer::~RLFrameBuffer()
	{
		rlDeleteFramebuffers(1, &m_rawData);
		rlCheckError();
	}
}