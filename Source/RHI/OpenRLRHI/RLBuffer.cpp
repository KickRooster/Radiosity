#include "RLBuffer.h"

namespace Core
{
	RLBuffer::RLBuffer(RLBufferTarget target)
	{
		m_target = target;

		rlGenBuffers(1, &m_rawData);
		rlCheckError();
	}

	RLbuffer RLBuffer::GetRawData() const
	{
		return m_rawData;
	}

	void RLBuffer::Activate() const
	{
		rlBindBuffer(m_target, m_rawData);
		rlCheckError();
	}

	void RLBuffer::Inactivate() const
	{
		rlBindBuffer(m_target, 0);
		rlCheckError();
	}

	void RLBuffer::AllocateMemorySpace(int32 size, RLBufferUsage usage)
	{
		rlBindBuffer(m_target, m_rawData);
		rlCheckError();

		rlBufferData(m_target, size, Null, usage);
		rlCheckError();

		rlBindBuffer(m_target, 0);
		rlCheckError();
	}

	void RLBuffer::UploadData(int32 offset, int32 size, void * pData)
	{
		rlBufferSubData(m_target, offset, size, pData);
		rlCheckError();
	}

	void * RLBuffer::Map(RLBufferAccessFlag accessFlag) const
	{
		void * pPixels;

		pPixels = rlMapBuffer(m_target, accessFlag);
		rlCheckError();

		return pPixels;
	}

	void RLBuffer::Unmap() const
	{
		rlUnmapBuffer(m_target);
		rlCheckError();

		rlBindBuffer(m_target, 0);
		rlCheckError();
	}

	/*
	void RLBuffer::Recreate(RLBufferTarget target, RLBufferUsage usage, int32 size)
	{
		if (m_rawData != Null)
		{
			rlDeleteBuffers(1, &m_rawData);
			rlCheckError();
		}

		rlGenBuffers(1, &m_rawData);
		rlCheckError();

		rlBindBuffer(target, m_rawData);
		rlCheckError();

		rlBufferData(target, size, Null, usage);
		rlCheckError();

		rlBindBuffer(target, 0);
		rlCheckError();
	}
	*/

	RLBuffer::~RLBuffer()
	{
		rlDeleteBuffers(1, &m_rawData);
		rlCheckError();
	}
}