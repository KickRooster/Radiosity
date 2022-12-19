#pragma once
#include "BaseGL.h"
#include "GLShader.h"

namespace Core
{
	class GLProgram
	{
	private:
		uint32 m_id;

	public:
		GLProgram();
		uint32 GetID() const;
		void AttachShader(GLShader * pShaderAttach);
		void DetachShader(GLShader * pShaderDetach);
		void BindFragDataLocation(uint32 attachIndex, const ANSICHAR * pName);
		void Link();
		void Activate() const;
		void ActivateTextureSlot(int32 slotIndex, const ANSICHAR * samplerName) const;
		void ActivateImageSlot(int32 slotIndex, const ANSICHAR * ImageName) const;
		void Set4f(const ANSICHAR * pName, float color[4]);
		void Inactivate() const;
		~GLProgram();
	};
}