#pragma once

//	???????
#include <memory>

//	????
#include <assert.h>

//	????????
#include "../Platform/Platform.h"

#define True 1
#define False 0
#define Null 0

const Core::int32 InvalidIndex = -1;

namespace Core
{
	//	???????,?????.
	extern ErrorCode ErrorCode_OK;

	struct InputState
	{
		Bool inlvaid = False;

		//	KeyboardState
		Bool wDown = False;
		Bool sDown = False;
		Bool aDown = False;
		Bool dDown = False;
		Bool qDown = False;
		Bool eDown = False;
		Bool leftShiftDown = False;
		
		//	MouseState
		Bool leftBuffonDown = False;
		Bool middleBuffonDown = False;
		Bool rightBuffonDown = False;
		uint32 xPos = 0;
		uint32 yPos = 0;
		uint32 zPos = 1;
	};

	struct WindowsState
	{
		int32 xPos = 0;
		int32 yPos = 0;
		int32 width = 0;
		int32 height = 0;
		ANSICHAR * titleName;
	};

	enum LightmapResolution
	{
		LightmapResolution_Invalid = -1,
		LightmapResolution_8x8,
		LightmapResolution_16x16,
		LightmapResolution_32x32,
		LightmapResolution_64x64,
		LightmapResolution_128x128,
		LightmapResolution_256x256,
		LightmapResolution_512x512,
		LightmapResolution_1024x1024,
		LightmapResolution_Count,
	};

	const ANSICHAR* LightmapResolutionItems[];

	int32 GetLightmapSideLength(LightmapResolution Resolution);

	enum LightPrecision
	{
		LightPrecision_Invalid = -1,
		LightPrecision_Low,
		LightPrecision_Middle,
		LightPrecision_High,
		LightPrecision_Count
	};

	const ANSICHAR* LightPrecisionItems[];

	float GetLightTriangleCountPerSide(LightPrecision Precision);

	int32 GetMinSizeLength(LightPrecision Precision);

	enum SuperSampleKernel
	{
		SuperSampleKernel_Invalid = -1,
		SuperSampleKernel_1x1,
		SuperSampleKernel_2x2,
		SuperSampleKernel_3x3,
		SuperSampleKernel_4x4,
		SuperSampleKernel_Count
	};

	const ANSICHAR* SuperSampleKernelItems[];

	int32 GetSuperSampleScale(SuperSampleKernel Kernel);
}
/*
//	??????????.
#define STDCALL __stdcall
#define CDECL __cdecl  
#define FASTCALL __fastcall
*/