#pragma once
#include "BaseRL.h"

namespace Core
{
	class RLBuffer
	{
	private:
		RLBufferTarget m_target;
		RLbuffer m_rawData;

	public:
		ANSICHAR * name;
		RLBuffer(RLBufferTarget target);
		RLbuffer GetRawData() const;
		void Activate() const;
		void Inactivate() const;
		void AllocateMemorySpace(int32 size, RLBufferUsage usage);
		void UploadData(int32 offset, int32 size, void * pData);
		void * Map(RLBufferAccessFlag accessFlag) const;
		void Unmap() const;
		~RLBuffer();
	};
}