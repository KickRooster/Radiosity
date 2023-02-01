#pragma once
#include <BaseInclude.h>
#include <string>

namespace Core
{
	class IFramework
	{
	public:
		IFramework() {}
		virtual void Initialize(int32 width, int32 height) = 0;
		virtual void Tick(float deltaTime, int32 width, int32 height, InputState & inputState) = 0;
		virtual void Render(int32 width, int32 height) = 0;
		virtual void Bake() = 0;
		virtual ~IFramework() {}
	};
}