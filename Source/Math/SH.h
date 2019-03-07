#pragma once
#include <BaseInclude.h>
#include "Vector3.h"

namespace Core
{
	struct SH4
	{
		float coefficients[4];

		static SH4 ProjectOntoSH4(Vector3 dir);
	};

	struct SH4Color
	{
		Vector3 coefficients[4];

		static SH4Color ProjectOntoSH4Color(Vector3 dir, Vector3 color);
	};
}