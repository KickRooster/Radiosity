#pragma once
#include "BaseRL.h"

namespace Core
{
	class RLTexture2D
	{
	private:
		RLtexture m_rawData;
		RLIinternalFormat m_internalFormat;
		RLPixelFormat m_pixelFormat;
		RLDataType m_dataType;
		int32 m_width;
		int32 m_height;
		RLTextureWrapMode m_wrapMode;
		RLTextureFilterMode m_filterMode;

		void setWrapMode(RLTextureWrapMode wrapMode);
		void setFilterMode(RLTextureFilterMode filterMode);

	public:
		RLTexture2D(RLIinternalFormat internalFormat, RLPixelFormat pixelFormat, RLDataType dataType, RLTextureWrapMode wrapMode, RLTextureFilterMode filterMode);
		RLtexture GetRawTexture();
		void Activate() const;
		void Inactivate() const;
		void LoadImage(int32 width, int32 height, const void * pData);
		void LoadClientImage(int32 width, int32 height, const void * pData);
		int32 GetDataSizePerPixel() const;
		void GetTexImage();
		void SetWrapMode(RLTextureWrapMode wrapMode);
		RLTextureWrapMode GetWrapMode() const;
		void SetFilterMode(RLTextureFilterMode filterMode);
		RLTextureFilterMode GetFilterMode() const;
		~RLTexture2D();
	};
}