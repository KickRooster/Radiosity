#include "RLTexture2D.h"

namespace Core
{
	void RLTexture2D::setWrapMode(RLTextureWrapMode wrapMode)
	{
		switch (wrapMode)
		{
		case Core::RLTextureWrapMode_Repeat:
			rlTexParameteri(RL_TEXTURE_2D, RL_TEXTURE_WRAP_S, RL_REPEAT);
			rlCheckError();
			rlTexParameteri(RL_TEXTURE_2D, RL_TEXTURE_WRAP_T, RL_REPEAT);
			rlCheckError();
			break;
		case Core::RLTextureWrapMode_Clamp:
			rlTexParameteri(RL_TEXTURE_2D, RL_TEXTURE_WRAP_S, RL_CLAMP_TO_EDGE);
			rlCheckError();
			rlTexParameteri(RL_TEXTURE_2D, RL_TEXTURE_WRAP_T, RL_CLAMP_TO_EDGE);
			rlCheckError();
			break;
		default:
			break;
		}
	}

	void RLTexture2D::setFilterMode(RLTextureFilterMode filterMode)
	{
		switch (filterMode)
		{
		case Core::RLTextureFilterMode_Point:
			rlTexParameteri(RL_TEXTURE_2D, RL_TEXTURE_MIN_FILTER, RL_NEAREST);
			rlCheckError();
			rlTexParameteri(RL_TEXTURE_2D, RL_TEXTURE_MAG_FILTER, RL_NEAREST);
			rlCheckError();
			break;
		case Core::RLTextureFilterMode_Bilinear:
			rlTexParameteri(RL_TEXTURE_2D, RL_TEXTURE_MIN_FILTER, RL_LINEAR);
			rlCheckError();
			rlTexParameteri(RL_TEXTURE_2D, RL_TEXTURE_MAG_FILTER, RL_LINEAR);
			rlCheckError();
			break;
		default:
			break;
		}
	}

	RLTexture2D::RLTexture2D(RLIinternalFormat internalFormat, RLPixelFormat pixelFormat, RLDataType dataType, RLTextureWrapMode wrapMode, RLTextureFilterMode filterMode)
		:
		m_internalFormat(internalFormat),
		m_pixelFormat(pixelFormat),
		m_dataType(dataType),
		m_wrapMode(wrapMode),
		m_filterMode(filterMode)
	{
		rlGenTextures(1, &m_rawData);
		rlCheckError();

		rlBindTexture(RL_TEXTURE_2D, m_rawData);
		rlCheckError();

		setWrapMode(wrapMode);
		setFilterMode(filterMode);

		rlBindTexture(RL_TEXTURE_2D, 0);
		rlCheckError();
	}

	RLtexture RLTexture2D::GetRawTexture()
	{
		return m_rawData;
	}

	void RLTexture2D::Activate() const
	{
		rlBindTexture(RL_TEXTURE_2D, m_rawData);
		rlCheckError();
	}

	void RLTexture2D::Inactivate() const
	{
		rlBindTexture(RL_TEXTURE_2D, 0);
		rlCheckError();
	}

	void RLTexture2D::LoadImage(int32 width, int32 height, const void * pData)
	{
		m_width = width;
		m_height = height;

		rlBindTexture(RL_TEXTURE_2D, m_rawData);
		rlCheckError();

		rlTexImage2D(
			RL_TEXTURE_2D,
			0,
			m_internalFormat,
			width,
			height,
			0,
			m_pixelFormat,
			m_dataType,
			pData);
		rlCheckError();

		rlBindTexture(RL_TEXTURE_2D, 0);
		rlCheckError();
	}

	void RLTexture2D::LoadClientImage(int32 width, int32 height, const void * pData)
	{
		m_width = width;
		m_height = height;

		rlPixelStorei(RL_UNPACK_CLIENT_STORAGE, RL_TRUE);
		rlCheckError();

		rlBindTexture(RL_TEXTURE_2D, m_rawData);
		rlCheckError();

		rlTexImage2D(
			RL_TEXTURE_2D,
			0,
			m_internalFormat,
			width,
			height,
			0,
			m_pixelFormat,
			m_dataType,
			pData);
		rlCheckError();

		rlPixelStorei(RL_UNPACK_CLIENT_STORAGE, RL_FALSE);
		rlCheckError();

		rlBindTexture(RL_TEXTURE_2D, 0);
		rlCheckError();
	}

	Core::int32 RLTexture2D::GetDataSizePerPixel() const
	{
		int32 multiplier = 1;

		switch (m_dataType)
		{
		case Core::RLDataType_Float:
			multiplier = sizeof(float);
			break;
		case Core::RLDataType_UnsignedByte:
			multiplier = sizeof(uint8);
			break;
		default:
			break;
		}

		int32 dataSizePerPixel = 1;

		switch (m_pixelFormat)
		{
		case Core::RLPixelFormat_RGB:
			dataSizePerPixel = multiplier * 3;
			break;
		case Core::RLPixelFormat_RGBA:
			dataSizePerPixel = multiplier * 4;
			break;
		case Core::RLPixelFormat_Luminance:
			dataSizePerPixel = multiplier * 1;
			break;
		default:
			break;
		}

		return dataSizePerPixel;
	}

	void RLTexture2D::GetTexImage()
	{
		rlGetTexImage(RL_TEXTURE_2D, 0, m_pixelFormat, m_dataType, Null);
		rlCheckError(); 
	}

	void RLTexture2D::SetWrapMode(RLTextureWrapMode wrapMode)
	{
		m_wrapMode = wrapMode;

		rlBindTexture(RL_TEXTURE_2D, m_rawData);
		rlCheckError();

		setWrapMode(wrapMode);

		rlBindTexture(RL_TEXTURE_2D, 0);
		rlCheckError();
	}

	Core::RLTextureWrapMode RLTexture2D::GetWrapMode() const
	{
		return m_wrapMode;
	}

	void RLTexture2D::SetFilterMode(RLTextureFilterMode filterMode)
	{
		m_filterMode = filterMode;

		rlBindTexture(RL_TEXTURE_2D, m_rawData);
		rlCheckError();

		setFilterMode(filterMode);

		rlBindTexture(RL_TEXTURE_2D, 0);
		rlCheckError();
	}

	Core::RLTextureFilterMode RLTexture2D::GetFilterMode() const
	{
		return m_filterMode;
	}

	RLTexture2D::~RLTexture2D()
	{
		rlDeleteTextures(1, &m_rawData);
		rlCheckError();
	}
}