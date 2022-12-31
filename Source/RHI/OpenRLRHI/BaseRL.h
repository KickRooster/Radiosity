#pragma once
#include <OpenRL/OpenRL.h>
#include <OpenRL/rl.h>
#include <BaseInclude.h>
#include STRING_INCLUDE_PATH

namespace Core
{
	enum RLTopology : RLenum
	{
		RLTopology_Triangles = RL_TRIANGLES,
		RLTopology_TriangleStrip = RL_TRIANGLE_STRIP,
	};

	enum RLBufferTarget : RLenum
	{
		RLBufferTarget_ArrayBuffer = RL_ARRAY_BUFFER,
		RLBufferTarget_ElementArrayBuffer = RL_ELEMENT_ARRAY_BUFFER,
		RLBufferTarget_PixelPackBuffer = RL_PIXEL_PACK_BUFFER,
		RLBufferTarget_PixelUnpackBuffer = RL_PIXEL_UNPACK_BUFFER,
		RLBufferTarget_UniformBlockBuffer = RL_UNIFORM_BLOCK_BUFFER,
	};

	enum RLBufferUsage : RLenum
	{
		RLBufferUsage_StaticDraw = RL_STATIC_DRAW,
		RLBufferUsage_DynamicDraw = RL_DYNAMIC_DRAW,
		RLBufferUsage_StreamDraw = RL_STREAM_DRAW,
		RLBufferUsage_StaticRead = RL_STATIC_READ,
		RLBufferUsage_DynamicRead = RL_DYNAMIC_READ,
		RLBufferUsage_StreamRead = RL_STREAM_READ,
		RLBufferUsage_StaticCopy = RL_STATIC_COPY,
		RLBufferUsage_DynamicCopy = RL_DYNAMIC_COPY,
	};

	enum RLBufferAccessFlag : RLenum
	{
		RLBufferAccessFlag_ReadOnly = RL_READ_ONLY,
		RLBufferAccessFlag_WriteOnly = RL_WRITE_ONLY,
		RLBufferAccessFlag_ReadWrite = RL_READ_WRITE,
	};

	enum RLIinternalFormat : RLenum
	{
		RLIinternalFormat_RGB = RL_RGB,
		RLIinternalFormat_RGBA = RL_RGBA,
		RLIinternalFormat_Luminance = RL_LUMINANCE,
	};

	enum RLPixelFormat : RLenum
	{
		RLPixelFormat_RGB = RL_RGB,
		RLPixelFormat_RGBA = RL_RGBA,
		RLPixelFormat_Luminance = RL_LUMINANCE,
	};

	enum RLDataType : RLenum
	{
		RLDataType_Float = RL_FLOAT,
		RLDataType_UnsignedByte = RL_UNSIGNED_BYTE,
		RLDataType_UnsignedInt = RL_UNSIGNED_INT,
	};

	enum RLAttachIndex : RLenum
	{
		RLAttachIndexColor0 = RL_COLOR_ATTACHMENT0,
	};

	enum RLShaderType : RLenum
	{
		RLShaderType_Invalid = -1,
		RLShaderType_VertexShader = RL_VERTEX_SHADER,
		RLShaderType_RayShader = RL_RAY_SHADER,
		RLShaderType_FrameShader = RL_FRAME_SHADER,
		RLShaderType_PrefixRayShader = RL_PREFIX_RAY_SHADER,
		RLShaderType_Count,
	};

	enum RLShaderStatus
	{
		RLShaderStatus_Undefined = -1,
		RLShaderStatus_OK,
		RLShaderStatus_CompileFailed,
	};

	enum RLTextureWrapMode
	{
		RLTextureWrapMode_Repeat,
		RLTextureWrapMode_Clamp,
	};

	enum RLTextureFilterMode
	{
		RLTextureFilterMode_Point,
		RLTextureFilterMode_Bilinear,
	};

	void ErrorCatcher(RLenum error, const void* privateData, size_t privateSize, const char* message, void* userData);

	RLenum rlCheckError_(const ANSICHAR *file, int32 line);

#define rlCheckError() rlCheckError_(__FILE__, __LINE__)
}