#include "RLVertexBufferLayout.h"

namespace Core
{
	RLVertexBufferLayout::RLVertexBufferLayout()
	{

	}

	void RLVertexBufferLayout::SetSlotElement(uint32 index, int32 size, RLDataType dataType, Bool normalized, uint32 stride, uint32 offset) const
	{
		//	rlGetAttribLocation(program, "positionAttribute")
		rlVertexAttribBuffer(index, size, dataType, normalized, stride, offset);
		rlCheckError();
	}

	RLVertexBufferLayout::~RLVertexBufferLayout()
	{

	}

}