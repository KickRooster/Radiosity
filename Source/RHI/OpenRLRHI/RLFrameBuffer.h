#pragma once
#include "BaseRL.h"
#include "RLTexture2D.h"

namespace Core
{
	class RLFrameBuffer
	{
		RLframebuffer m_rawData;

	public:
		RLFrameBuffer();
		void AttachColor(RLAttachIndex attachIndex, RLTexture2D * pColorTexture);
		void Activate() const;
		void Inactivate() const;
		~RLFrameBuffer();
	};
}