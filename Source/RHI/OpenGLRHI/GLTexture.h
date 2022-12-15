#pragma once
#include <BaseInclude.h>
#include "BaseGL.h"

namespace Core
{
	class GLTexture
	{
	private:
		uint32 m_id;
		GLTextureTarget m_target;
		GLInternalFormat m_internalFormat;
		GLPixelFormat m_pixelFormat;
		GLDataType m_dataType;
		GLTextureWrapMode m_wrapMode;
		GLTextureFilterMode m_filterMode;
		void setWrapMode(GLTextureWrapMode wrapMode);
		void setFilterMode(GLTextureFilterMode filterMode);

	public:
		GLTexture(GLTextureTarget target, GLInternalFormat internalFormat, GLPixelFormat pixelFormat, GLDataType dataType, GLTextureWrapMode wrapMode, GLTextureFilterMode filterMode);
		uint32 GetID() const;
		uint64 GetID64() const;
		GLTextureTarget GetTarget() const;
		void Activate() const;
		void LoadImage(int32 width, int32 height, const void * pData);
		void Fetch(void * pData);
		void Inactivate() const;
		void SetWrapMode(GLTextureWrapMode wrapMode);
		GLTextureWrapMode GetWrapMode() const;
		void SetFilterMode(GLTextureFilterMode filterMode);
		GLTextureFilterMode GetFilterMode() const;
		int32 GetDataSizePerPixel() const;
		~GLTexture();
	};
}