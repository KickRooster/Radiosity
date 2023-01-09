#pragma once

//	???????
#include <memory>

//	????
#include <assert.h>

//	????????
#include "../Platform/Platform.h"

//	?????,??????????STL??
#define ctd std

#define VECTOR_INCLUDE_PATH <vector>
#define SET_INCLUDE_PATH <set>
#define MAP_INCLUDE_PATH <map>
#define HASH_MAP_INCLUDE_PATH <hash_map>
#define HASH_SET_INCLUDE_PATH <hash_set>
#define STRING_INCLUDE_PATH <string>
#define LIST_INCLUDE_PATH <list>
#define STACK_INCLUDE_PATH <stack>
#define FUNCTIONAL_INCLUDE_PATH <functional>
#define ALGORITHM_INCLUDE_PATH <algorithm>

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
}
/*
//	????????��.
#define STDCALL __stdcall
#define CDECL __cdecl  
#define FASTCALL __fastcall
*/