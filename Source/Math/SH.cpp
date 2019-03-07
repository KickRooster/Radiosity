#include "SH.h"

namespace Core
{
	Core::SH4 SH4::ProjectOntoSH4(Vector3 dir)
	{
		SH4 sh;

		// Band 0
		sh.coefficients[0] = 0.282095f;

		// Band 1
		sh.coefficients[1] = -0.488603f * dir.y;
		sh.coefficients[2] = 0.488603f * dir.z;
		sh.coefficients[3] = -0.488603f * dir.x;

		return sh;
	}

	Core::SH4Color SH4Color::ProjectOntoSH4Color(Vector3 dir, Vector3 color)
	{
		SH4 sh = SH4::ProjectOntoSH4(dir);

		SH4Color shColor;

		for (uint64 i = 0; i < 4; ++i)
			shColor.coefficients[i] = color * sh.coefficients[i];

		return shColor;
	}
}