#pragma once
#include "..\..\..\RHI\OpenGLRHI\OpenGLDevice.h"
#include "..\..\..\Framework\IFramework.h"


namespace Core
{
	class WindowsRuntime : public IFramework
	{
	public:
		WindowsRuntime();
		virtual void Initialize(int32 width, int32 height) override;
		virtual void Tick(float deltaTime, int32 width, int32 height, InputState & inputState) override;
		virtual void Render(int32 width, int32 height) override;
		virtual ~WindowsRuntime();
	};
}