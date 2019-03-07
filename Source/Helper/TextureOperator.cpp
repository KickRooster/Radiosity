#include "TextureOperator.h"

namespace Core
{
	TextureOperator::TextureOperator()
	{
	}

	void TextureOperator::LoadTexture(const ANSICHAR * fileFullPathName, TextureFormat format, Texture * pOutTexture)
	{
		SDL_Surface* pRawSurface = IMG_Load(fileFullPathName);

		if (pRawSurface)
		{
			SDL_PixelFormat * pFormat = pRawSurface->format;

			pOutTexture->width = pRawSurface->w;
			pOutTexture->height = pRawSurface->h;

			if (format == TextureFormat_RGBA32 && pRawSurface->format->BytesPerPixel == 3)
			{
				format = TextureFormat_RGB24;
				pOutTexture->info->format = TextureFormat_RGB24;
			}
			else if (format == TextureFormat_SRGBA32 && pRawSurface->format->BytesPerPixel == 3)
			{
				format = TextureFormat_SRGB24;
				pOutTexture->info->format = TextureFormat_SRGB24;
			}

			Uint32 pixelFormat;

			switch (format)
			{
			case Core::TextureFormat_RGB24:
			case Core::TextureFormat_SRGB24:
				pixelFormat = SDL_PIXELFORMAT_RGB24;
				break;
			case Core::TextureFormat_RGBA32:
			case Core::TextureFormat_SRGBA32:
				pixelFormat = SDL_PIXELFORMAT_RGBA32;
				break;
			default:
				pixelFormat = SDL_PIXELFORMAT_UNKNOWN;
				break;
			}

			SDL_Surface* pRGBSurface = SDL_ConvertSurfaceFormat(pRawSurface, pixelFormat, 0);

			SDL_FreeSurface(pRawSurface);

			pOutTexture->pImage = new uint8[pOutTexture->width *
				pOutTexture->height *
				sizeof(uint8) * pRGBSurface->format->BytesPerPixel];

			memcpy(pOutTexture->pImage,
				pRGBSurface->pixels,
				pOutTexture->width * pOutTexture->height * sizeof(uint8) * pRGBSurface->format->BytesPerPixel);
			
			SDL_FreeSurface(pRGBSurface);
		}
	}

	void TextureOperator::SavePNG(const ANSICHAR * fileFullPathName, TextureFormat textureFormat, uint8 * pPixels, int32 width, int32 height)
	{
		uint32 rmask;
		uint32 gmask;
		uint32 bmask;
		uint32 amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		int shift = (textureFormat == TextureFormat_RGB24) ? 8 : 0;
		rmask = 0xff000000 >> shift;
		gmask = 0x00ff0000 >> shift;
		bmask = 0x0000ff00 >> shift;
		amask = 0x000000ff >> shift;
#else // little endian, like x86
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif

		int32 depth = textureFormat == TextureFormat_RGBA32 ? 32 : 24;
		int32 pitch = textureFormat == TextureFormat_RGBA32 ? 4 * width : 3 * width;

		SDL_Surface* pSurface = SDL_CreateRGBSurfaceFrom((void*)pPixels, width, height, depth, pitch,
			rmask, gmask, bmask, amask);

		if (pSurface == Null)
			return;

		IMG_SavePNG(pSurface, fileFullPathName);

		SDL_FreeSurface(pSurface);
	}

	TextureOperator::~TextureOperator()
	{
	}
}