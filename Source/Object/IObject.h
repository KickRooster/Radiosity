#pragma once
#include "..\RHI\OpenGLRHI\OpenGLDevice.h"
#include "..\RHI\OpenRLRHI\RLDevice.h"

namespace Core
{
	class IObject
	{
	public:
		virtual void Dummy() = 0;
		virtual void Tick(float deltaTime, OpenGLDevice * pDevice, RLDevice * pRLDevice) = 0;
	};
}