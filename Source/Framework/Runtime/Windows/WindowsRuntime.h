#pragma once
#include "..\..\..\RHI\OpenGLRHI\OpenGLDevice.h"
#include "../../IFramework.h"

namespace Core
{
	class WindowsRuntime : public IFramework
	{
	public:
		WindowsRuntime();
		void Initialize();
		virtual void Tick(float deltaTime, int32 width, int32 height);
		virtual void Render(int32 width, int32 height);
		virtual ~WindowsRuntime();
	};
}