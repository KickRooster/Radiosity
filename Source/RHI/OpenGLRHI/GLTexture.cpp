#include "GLTexture.h"

namespace Core
{

	void GLTexture::setWrapMode(GLTextureWrapMode wrapMode)
	{
		switch (m_wrapMode)
		{
		case Core::GLTextureWrapMode_Repeat:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glCheckError();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glCheckError();
			break;
		case Core::GLTextureWrapMode_Clamp:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glCheckError();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glCheckError();
			break;
		default:
			break;
		}
	}

	void GLTexture::setFilterMode(GLTextureFilterMode filterMode)
	{
		switch (m_filterMode)
		{
		case Core::GLTextureFilterMode_Point:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glCheckError();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glCheckError();
			break;
		case Core::GLTextureFilterMode_Bilinear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glCheckError();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

		glBindTexture(GL_TEXTURE_2D, m_id);
		glCheckError();

		setWrapMode(m_wrapMode);
		setFilterMode(m_filterMode);

		glBindTexture(GL_TEXTURE_2D, 0);
		glCheckError();

		//	Test Code
		//uint32 testID;
		//
		//glGenTextures(1, &testID);
		//glCheckError();
		//
		//glBindTexture(GL_TEXTURE_2D, testID);
		//glCheckError();
		//
		//glTexImage2D(
		//	GL_TEXTURE_2D,
		//	0,
		//	GL_DEPTH24_STENCIL8,
		//	10,
		//	1,
		//	0,
		//	GL_DEPTH_STENCIL,
		//	GL_UNSIGNED_INT_24_8,//GL_FLOAT,
		//	Null);
		//glCheckError();
		//
		//int32 val = 1 << 30 | (uint32)(1) << 17;
		////float val[] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f };
		//
		//glTexSubImage2D(
		//	GL_TEXTURE_2D,
		//	0,
		//	0,
		//	0,
		//	10,
		//	1,
		//	GL_DEPTH_STENCIL,//GL_DEPTH_COMPONENT,// GL_DEPTH_STENCIL,
		//	GL_UNSIGNED_INT_24_8,// GL_UNSIGNED_INT_24_8,//GL_FLOAT,	
		//	&val);
		//glCheckError();
		//
		//int32 val2 = 0;
		////float val2[10];
		//
		//glGetTexImage(
		//	GL_TEXTURE_2D,
		//	0,
		//	GL_DEPTH_STENCIL,//GL_DEPTH_COMPONENT,//GL_DEPTH_STENCIL,
		//	GL_UNSIGNED_INT_24_8,//GL_UNSIGNED_INT_24_8,//GL_FLOAT,
		//	&val2
		//);
		//glCheckError();
	}

	Core::uint32 GLTexture::GetID() const
	{
		return m_id;
	}

	Core::uint64 GLTexture::GetID64() const
	{
		return m_id;
	}

	void GLTexture::Activate() const
	{
		glBindTexture(m_target, m_id);
		glCheckError();
	}

	void GLTexture::LoadImage(uint32 width, uint32 height, const void * pData)
	{
		glBindTexture(m_target, m_id);
		glCheckError();

		glTexImage2D(m_target, 0, m_internalFormat, width, height, 0, m_pixelFormat, m_dataType, pData);
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

	Core::GLTextureWrapMode GLTexture::GetWrapMode() const
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

	Core::GLTextureFilterMode GLTexture::GetFilterMode() const
	{
		return m_filterMode;
	}

	int32 GLTexture::GetDataSizePerPixel() const
	{
		int32 multiplier = 1;

		switch (m_dataType)
		{
		case Core::GLDataType_Float:
			multiplier = sizeof(float);
			break;
		case Core::GLDataType_UnsignedByte:
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