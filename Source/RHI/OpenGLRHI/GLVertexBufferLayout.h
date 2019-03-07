#pragma once
#include "BaseGL.h"
#include "GLBuffer.h"

namespace Core
{
	class GLVertexBufferLayout
	{
	private:
		uint32 m_vaoID;

	public:
		GLVertexBufferLayout();
		void Activate() const;
		void SetSlotElement(uint32 index, int32 size, GLDataType dataType, Bool normalized, int32 stride, const GLvoid * pointer) const;
		void Inactivate() const;
		~GLVertexBufferLayout();
	};
}