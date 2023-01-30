#pragma once
#include "BaseGL.h"

namespace Core
{
	enum BufferTarget
	{
		BufferTarget_ArrayBuffer = GL_ARRAY_BUFFER,
		BufferTarget_ElementArray = GL_ELEMENT_ARRAY_BUFFER,
		BufferTarget_UniformBuffer = GL_UNIFORM_BUFFER,
		BufferTarget_PixelPackBuffer = GL_PIXEL_PACK_BUFFER,
	};

	enum AccessFlag
	{
		AccessFlag_ReadOnly = GL_READ_ONLY,
		AccessFlag_WriteOnly = GL_WRITE_ONLY,
		AccessFlag_ReadWrite = GL_READ_WRITE,
	};

	enum BufferUsage
	{
		BufferUsage_StaticDraw = GL_STATIC_DRAW,
		BufferUsage_DynamicDraw = GL_DYNAMIC_DRAW,
		BufferUsage_StreamRead = GL_STREAM_READ,
		BufferUsage_StreamCopy = GL_STREAM_COPY,
	};

	class GLBuffer
	{
		uint32 m_id;

	public:
		GLBuffer();
		uint32 GetID()const;
		void Recreate(BufferTarget target, BufferUsage usage, int32 size);
		void Activate(BufferTarget target) const;
		void UploadData(BufferTarget target, int32 offset, int32 size, void * pData);
		void Inactivate(BufferTarget target) const;
		void * Map(BufferTarget target, AccessFlag access);
		void Unmap(BufferTarget target);
		void ReadPixels(int32 xOffset, int32 yOffset, int32 width, int32 height, GLPixelFormat pixelFormat, GLDataType dataType, void * pOffset);
		~GLBuffer();
	};
}