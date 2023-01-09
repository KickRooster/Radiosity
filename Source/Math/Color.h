#pragma once
#include <BaseInclude.h>

namespace Core
{
	struct Color
	{
		uint8 R;
		uint8 G;
		uint8 B;
		uint8 A;

		Color();
		Color(uint8 InR, uint8 InG, uint8 InB, uint8 InA);
		Color& operator*=(uint8 Scale);
	};
}