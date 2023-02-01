#include "GLTexture.h"

namespace Core
{
	void GLTexture::setWrapMode(GLTextureWrapMode wrapMode)
	{
		m_wrapMode = wrapMode;
		
		switch (m_wrapMode)
		{
		case GLTextureWrapMode_Repeat:
			glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glCheckError();
			glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glCheckError();
			if (m_target == GLTextureTarget_3D || m_target == GLTextureTarget_CubeMAP)
			{
				glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_REPEAT);
				glCheckError();
			}
			break;
		case GLTextureWrapMode_Clamp:
			glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glCheckError();
			glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_CLAMP);
			glCheckError();
			if (m_target == GLTextureTarget_3D || m_target == GLTextureTarget_CubeMAP)
			{
				glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glCheckError();
			}
			break;
		default:
			break;
		}
	}

	void GLTexture::setFilterMode(GLTextureFilterMode filterMode)
	{
		m_filterMode = filterMode;
		
		switch (m_filterMode)
		{
		case GLTextureFilterMode_Point:
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glCheckError();
			glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glCheckError();
			break;
		case GLTextureFilterMode_Bilinear:
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glCheckError();
			glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glCheckError();
			break;
		default:
			break;
		}
	}

	GLTexture::GLTexture(GLTextureTarget target, GLInternalFormat internalFormat, GLPixelFormat pixelFormat, GLDataType dataType, GLTextureWrapMode wrapMode, GLTextureFilterMode filterMode)
		:
		m_target(target),
		m_internalFormat(internalFormat),
		m_pixelFormat(pixelFormat),
		m_dataType(dataType),
		m_wrapMode(wrapMode),
		m_filterMode(filterMode)
	{
		glGenTextures(1, &m_id);
		glCheckError();

		glBindTexture(target, m_id);
		glCheckError();

		setWrapMode(m_wrapMode);
		setFilterMode(m_filterMode);

		glBindTexture(target, 0);
		glCheckError();
	}
	
	uint32 GLTexture::GetID() const
	{
		return m_id;
	}

	uint64 GLTexture::GetID64() const
	{
		return m_id;
	}

	GLTextureTarget GLTexture::GetTarget() const
	{
		return m_target;
	}

	void GLTexture::Activate() const
	{
		glBindTexture(m_target, m_id);
		glCheckError();
	}

	void GLTexture::LoadImage(int32 width, int32 height, const void * pData)
	{
		glBindTexture(m_target, m_id);
		glCheckError();

		if (m_target == GLTextureTarget_CubeMAP)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,m_internalFormat,width,height,0,m_pixelFormat,m_dataType,pData);
			glCheckError();
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,m_internalFormat,width,height,0,m_pixelFormat,m_dataType,pData);
			glCheckError();
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,m_internalFormat,width,height,0,m_pixelFormat,m_dataType,pData);
			glCheckError();
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,m_internalFormat,width,height,0,m_pixelFormat,m_dataType,pData);
			glCheckError();
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,m_internalFormat,width,height,0,m_pixelFormat,m_dataType,pData);
			glCheckError();
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,m_internalFormat,width,height,0,m_pixelFormat,m_dataType,pData);
			glCheckError();
		}
		else
		{
			glTexImage2D(m_target, 0, m_internalFormat, width, height, 0, m_pixelFormat, m_dataType, pData);
			glCheckError();
		}
			
		glBindTexture(m_target, 0);
		glCheckError();
	}

	void GLTexture::GenerateMipMap()
	{
		glBindTexture(m_target, m_id);
		glCheckError();

		glGenerateMipmap(m_target);
		glCheckError();

		glBindTexture(m_target, 0);
		glCheckError();
	}
	
	void GLTexture::Fetch(void * pData)
	{
		glBindTexture(m_target, m_id);
		glCheckError();

		glGetTexImage(m_target, 0, m_pixelFormat, m_dataType, pData);
		glCheckError();

		glBindTexture(m_target, 0);
		glCheckError();
	}

	void GLTexture::Inactivate() const
	{
		glBindTexture(m_target, 0);
		glCheckError();
	}

	void GLTexture::SetWrapMode(GLTextureWrapMode wrapMode)
	{
		m_wrapMode = wrapMode;

		glBindTexture(m_target, m_id);
		glCheckError();

		setWrapMode(wrapMode);

		glBindTexture(m_target, 0);
		glCheckError();
	}

	GLTextureWrapMode GLTexture::GetWrapMode() const
	{
		return m_wrapMode;
	}

	void GLTexture::SetFilterMode(GLTextureFilterMode filterMode)
	{
		m_filterMode = filterMode;

		glBindTexture(m_target, m_id);
		glCheckError();

		setFilterMode(filterMode);

		glBindTexture(m_target, 0);
		glCheckError();
	}

	GLTextureFilterMode GLTexture::GetFilterMode() const
	{
		return m_filterMode;
	}

	int32 GLTexture::GetDataSizePerPixel() const
	{
		int32 multiplier = 1;

		switch (m_dataType)
		{
		case GLDataType_Float:
			multiplier = sizeof(float);
			break;
		case GLDataType_UnsignedByte:
			multiplier = sizeof(uint8);
			break;
		default:
			break;
		}

		int32 dataSizePerPixel = 1;

		switch (m_pixelFormat)
		{
		case GLPixelFormat_RGB:
			dataSizePerPixel = multiplier * 3;
			break;
		case GLPixelFormat_RGBA:
			dataSizePerPixel = multiplier * 4;
			break;
		default:
			break;
		}

		return dataSizePerPixel;
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures(1, &m_id);
		glCheckError();
	}
}