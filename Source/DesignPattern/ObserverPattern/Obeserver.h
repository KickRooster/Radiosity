#pragma once
#include <BaseInclude.h>

namespace Core
{
	class Obeserver
	{
	public:
		Obeserver() {}
		virtual void UpdateAhead() = 0;
		virtual void UpdateBehind() = 0;
		virtual ~Obeserver() {}
	};
}