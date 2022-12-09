#pragma once
#include <GL/glew.h>
#include <BaseInclude.h>
#include STRING_INCLUDE_PATH

namespace Core
{
	GLenum glCheckError_(const ANSICHAR *file, int32 line);

#define glCheckError() glCheckError_(__FILE__, __LINE__)

	const uint32 InvalidID = 1 << 31;

	enum GLDataType
	{
		GLDataType_Byte = GL_BYTE,
		GLDataType_Float = GL_FLOAT,
		GLDataType_UnsignedByte = GL_UNSIGNED_BYTE,
		GLDataType_UnsignedShort = GL_UNSIGNED_SHORT,
		GLDataType_UnsignedInt = GL_UNSIGNED_INT,
	};

	//	��������ȫ�ֵ�uniform buffer������ʵ����uniform buffer
	//	ȫ�Ӹ���Ƶ�ʶ���.
	enum GLShaderDataAlias : int32
	{
		GLShaderDataAlias_CameraMatrices,
		GLShaderDataAlias_ObjectMatrices,
		GLShaderDataAlias_ObjectMatricesIT,
		GLShaderDataAlias_LightmapUVParam,
		GLShaderDataAlias_PostprocessParam,
		GLShaderDataAlias_CameraPos,
		GLShaderDataAlias_LightParam,
	};

	enum GLTopology
	{
		GLTopology_Triangles = GL_TRIANGLES,
	};

	enum GLTextureTarget
	{
		GLTextureTarget_1D = GL_TEXTURE_1D,
		GLTextureTarget_2D = GL_TEXTURE_2D,
		GLTextureTarget_3D = GL_TEXTURE_3D,
	};

	enum GLInternalFormat
	{
		GLInternalFormat_Depth = GL_DEPTH_COMPONENT,
		GLInternalFormat_DepthStencil = GL_DEPTH_STENCIL,
		GLInternalFormat_R = GL_RED,
		GLInternalFormat_RG = GL_RG,
		GLInternalFormat_RGB = GL_RGB,
		GLInternalFormat_RGBA = GL_RGBA,
		GLInternalFormat_RGB32F = GL_RGB32F,
		GLInternalFormat_RGBA32F = GL_RGBA32F,
		GLInternalFormat_SRGB8 = GL_SRGB8,
		GLInternalFormat_SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
	};

	enum GLPixelFormat
	{
		GLPixelFormat_R = GL_RED,
		GLPixelFormat_RG = GL_RG,
		GLPixelFormat_RGB = GL_RGB,
		GLPixelFormat_BGR = GL_BGR,
		GLPixelFormat_RGBA = GL_RGBA,
		GLPixelFormat_BGRA = GL_BGRA,
		GLPixelFormat_RI = GL_RED_INTEGER,
		GLPixelFormat_RGI = GL_RG_INTEGER,
		GLPixelFormat_RGBI = GL_RGB_INTEGER,
		GLPixelFormat_BGRI = GL_BGR_INTEGER,
		GLPixelFormat_RRGAI = GL_RGBA_INTEGER,
		GLPixelFormat_BGRAI = GL_BGRA_INTEGER,
		GLPixelFormat_StencilIndex = GL_STENCIL_INDEX,
		GLPixelFormat_Depth = GL_DEPTH_COMPONENT,
		GLPixelFormat_Stencil = GL_DEPTH_STENCIL,
	};

	enum GLShaderType
	{
		GLShaderType_VertexShader = GL_VERTEX_SHADER,
		GLShaderType_FragmentShader = GL_FRAGMENT_SHADER,
	};

	enum GLShaderStatus
	{
		GLShaderStatus_Undefined = -1,
		GLShaderStatus_OK,
		GLShaderStatus_CompileFailed,
	};

	enum GLAttachIndex : GLenum
	{
		GLAttachIndexColor0 = GL_COLOR_ATTACHMENT0,
		GLAttachIndexColor1 = GL_COLOR_ATTACHMENT1,
		GLAttachIndexColor2 = GL_COLOR_ATTACHMENT2,
		GLAttachIndexColor3 = GL_COLOR_ATTACHMENT3,
	};

	enum GLTextureWrapMode
	{
		GLTextureWrapMode_Repeat,
		GLTextureWrapMode_Clamp,
	};

	enum GLTextureFilterMode
	{
		GLTextureFilterMode_Point,
		GLTextureFilterMode_Bilinear,
	};
}