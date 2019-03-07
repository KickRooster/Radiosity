#pragma once
#include "BaseGL.h"

namespace Core
{
	class GLFragmentShader
	{
		static ANSICHAR * defaultShaderText;

		uint32 m_id;

	public:
		GLFragmentShader();
		void Compile() const;
		uint32 GetID() const;
		~GLFragmentShader();
	};
}