#include "ImageTexture.h"

namespace Core
{
	GLImageTexture::GLImageTexture(GLTextureTarget target, GLInternalFormat internalFormat, GLPixelFormat pixelFormat, GLDataType dataType, GLTextureWrapMode wrapMode, GLTextureFilterMode filterMode,  GLImageUnit Unit, int32 BoundLevel, GLImageAccess Access)
		:
		GLTexture(target, internalFormat, pixelFormat, dataType, wrapMode, filterMode),
		m_unit(Unit)
	{
		glBindImageTexture(Unit, m_id, BoundLevel, False, 0, Access, m_internalFormat);
		glCheckError();
	}
	
	GLImageUnit GLImageTexture::GetUnit() const
	{
		return m_unit;
	}
	
	GLImageTexture::~GLImageTexture()
	{
	}
}