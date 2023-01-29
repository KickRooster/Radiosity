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

	const ANSICHAR* LightPrecisionItems[] = {
		"Low",
		"Middle",
		"High",
	};
	
	float GetLightTriangleCountPerSide (LightPrecision Precision)
	{
		float TriangleCountPerSide ;
		
		switch (Precision)
		{
		case LightPrecision_Low:
			TriangleCountPerSide = 0.00390625f;
			break ;
		case LightPrecision_Middle:
			TriangleCountPerSide = 0.0078125f;
			break ;
		case LightPrecision_High:
			TriangleCountPerSide = 0.015625f;
			break ;
		default:
			TriangleCountPerSide = 0;
			break;
		}

		return TriangleCountPerSide;
	}

	int32 GetMinSizeLength(LightPrecision Precision)
	{
		float TriangleCountPerSize = GetLightTriangleCountPerSide(Precision);

		float MinSizeLength = 1.0f / TriangleCountPerSize;

		if (MinSizeLength < 1.0f)
		{
			MinSizeLength = 1.0 ;
		}

		return static_cast<int32>(MinSizeLength);
	}

	const ANSICHAR* SuperSampleKernelItems[] = {
		"1x1",
		"2x2",
		"3x3",
		"4x4",
	};

	int32 GetSuperSampleScale(SuperSampleKernel Kernel)
	{
		int32 Scale;

		switch (Kernel)
		{
		case SuperSampleKernel_1x1:
			Scale = 1;
			break;
		case SuperSampleKernel_2x2:
			Scale = 2;
			break;
		case SuperSampleKernel_3x3:
			Scale = 3;
			break;
		case SuperSampleKernel_4x4:
			Scale = 4;
			break;
		default:
			Scale = 0;
			break;
		}

		return Scale;
	}
}