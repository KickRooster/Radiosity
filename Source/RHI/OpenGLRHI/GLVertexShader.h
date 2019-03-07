#pragma once
#include "BaseGL.h"

namespace Core
{
	class GLVertexShader
	{
		static ANSICHAR * defaultShaderText;

		uint32 m_id;

	public:
		GLVertexShader();
		void Compile() const;
		uint32 GetID() const;
		~GLVertexShader();
	};
}