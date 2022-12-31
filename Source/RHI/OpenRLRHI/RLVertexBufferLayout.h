#pragma once
#include "BaseRL.h"

namespace Core
{
	class RLVertexBufferLayout
	{
	public:
		RLVertexBufferLayout();
		void SetSlotElement(uint32 index, int32 size, RLDataType dataType, Bool normalized, uint32 stride, uint32 offset) const;
		~RLVertexBufferLayout();
	};
}