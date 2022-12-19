#include "ImageTexture.h"

namespace Core
{
	void GLImageTexture::setWrapMode(GLTextureWrapMode wrapMode)
	{
		switch (wrapMode)
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

	void GLImageTexture::setFilterMode(GLTextureFilterMode filterMode)
	{
		switch (filterMode)
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
	
	GLImageTexture::GLImageTexture(GLTextureTarget target, GLInternalFormat internalFormat, GLPixelFormat pixelFormat, GLDataType dataType, GLTextureWrapMode wrapMode, GLTextureFilterMode filterMode,  GLImageUnit Unit, int32 BoundLevel, GLImageAccess Access)
		:
		m_target(target),
		m_internalFormat(internalFormat),
		m_pixelFormat(pixelFormat),
		m_dataType(dataType),
		m_unit(Unit)
	{
		glGenTextures(1, &m_id);
		glCheckError();

		glBindTexture(target, m_id);
		glCheckError();

		setWrapMode(wrapMode);
		setFilterMode(filterMode);
		
		glBindTexture(target, 0);
		glCheckError();
		
		glBindImageTexture(Unit, m_id, BoundLevel, False, 0, Access, m_internalFormat);
		glCheckError();
	}
	
	uint32 GLImageTexture::GetID() const
	{
		return m_id;
	}

	uint64 GLImageTexture::GetID64() const
	{
		return m_id;
	}

	GLTextureTarget GLImageTexture::GetTarget() const
	{
		return m_target;
	}

	GLImageUnit GLImageTexture::GetUnit() const
	{
		return m_unit;
	}
	
	void GLImageTexture::LoadImage(int32 width, int32 height, const void * pData)
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

	void GLImageTexture::Fetch(void * pData)
	{
		glBindTexture(m_target, m_id);
		glCheckError();

		glGetTexImage(m_target, 0, m_pixelFormat, m_dataType, pData);
		glCheckError();

		glBindTexture(m_target, 0);
		glCheckError();
	}
	
	int32 GLImageTexture::GetDataSizePerPixel() const
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

	GLImageTexture::~GLImageTexture()
	{
		glDeleteTextures(1, &m_id);
		glCheckError();
	}
}