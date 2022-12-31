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

	void RLFrameBuffer::Activate() const
	{
		rlBindFramebuffer(RL_FRAMEBUFFER, m_rawData);
		rlCheckError();
		
		RLenum Status = rlCheckFramebufferStatus(RL_FRAMEBUFFER);
		rlCheckError();
		
		if (Status != RL_FRAMEBUFFER_COMPLETE)
		{
			assert(False);
		}
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