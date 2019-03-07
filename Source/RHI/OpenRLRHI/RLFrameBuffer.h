#pragma once
#include "BaseRL.h"
#include "RLTexture2D.h"

namespace Core
{
	class RLFrameBuffer
	{
	private:
		uint32 m_width;
		uint32 m_height;
		RLframebuffer m_rawData;

	public:
		RLFrameBuffer();
		void AttachColor(RLAttachIndex attachIndex, RLTexture2D * pColorTexture);
		void Resize(uint32 width, uint32 height);
		uint32 GetWidth() const;
		uint32 GetHeight() const;
		void Activate() const;
		void Inactivate() const;
		~RLFrameBuffer();
	};
}