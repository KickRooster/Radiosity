#include "BaseInclude.h"

namespace Core
{
	//	???????,?????.
	ErrorCode ErrorCode_OK = 0;

	const ANSICHAR* LightmapResolutionItems[] = {
		"8x8",
		"16x16",
		"32x32",
		"64x64",
		"128x128",
		"256x256",
		"512x512",
		"1024x1024",
	};

	int32 GetLightmapSideLength(LightmapResolution Resolution)
	{
		int32 SizeLength;
		
		switch (Resolution)
		{
		case LightmapResolution_8x8:
			SizeLength = 8;
			break;
		case LightmapResolution_16x16:
			SizeLength = 16;
			break;
		case LightmapResolution_32x32:
			SizeLength = 32;
			break;
		case LightmapResolution_64x64:
			SizeLength = 64;
			break;
		case LightmapResolution_128x128:
			SizeLength = 128;
			break;
		case LightmapResolution_256x256:
			SizeLength = 256;
			break;
		case LightmapResolution_512x512:
			SizeLength = 512;
			break;
		case LightmapResolution_1024x1024:
			SizeLength = 1024;
			break;
		default:
			SizeLength = 0;
			break ;
		}

		return SizeLength;
	}
}