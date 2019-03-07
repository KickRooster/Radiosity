#pragma once
#include <BaseInclude.h>
#include <SDL/SDL_image.h>
#include "../RHI/OpenGLRHI/GLTexture.h"
#include "../Asset/Texture.h"

#include STRING_INCLUDE_PATH

namespace Core
{
	class TextureOperator
	{
	public:
		TextureOperator();
		static void LoadTexture(const ANSICHAR * fileFullPathName, TextureFormat format, Texture * pOutTexture);
		static void SavePNG(const ANSICHAR * fileFullPathName, TextureFormat textureFormat, uint8 * pPixels, int32 width, int32 height);
		~TextureOperator();
	};
}