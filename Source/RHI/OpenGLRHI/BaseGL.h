#pragma once
#include <GL/glew.h>
#include <BaseInclude.h>
#include <string>

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
		GLShaderDataAlias_CameraUniformData,
		GLShaderDataAlias_GlobalRenderData,
		GLShaderDataAlias_ShooterInfo,
		GLShaderDataAlias_ObjectMatrices,
		GLShaderDataAlias_ObjectMatricesIT,
		GLShaderDataAlias_LightmapUVParam,
		GLShaderDataAlias_PostprocessParam,
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
		GLTextureTarget_CubeMAP = GL_TEXTURE_CUBE_MAP,
		GLTextureTarget_CubeMap_Positive_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GLTextureTarget_CubeMap_Negative_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GLTextureTarget_CubeMap_Positive_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GLTextureTarget_CubeMap_Negative_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GLTextureTarget_CubeMap_Positive_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GLTextureTarget_CubeMap_Negative_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
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

	enum GLImageUnit
	{
		GLImageUnit_0 = 0,
		GLImageUnit_1 = 1,
		GLImageUnit_2 = 2,
		GLImageUnit_3 = 3,
		GLImageUnit_4 = 4,
		GLImageUnit_5 = 5,
		GLImageUnit_6 = 6,
		GLImageUnit_7 = 7,
	};

	enum GLImageAccess
	{
		GLImageAccess_ReadOnly = GL_READ_ONLY,
		GLImageAccess_WriteOnly = GL_WRITE_ONLY,
		GLImageAccess_ReadWrite = GL_READ_WRITE,
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
		GLAttachIndexColor4 = GL_COLOR_ATTACHMENT4,
		GLAttachIndexColor5 = GL_COLOR_ATTACHMENT5,
		GLAttachIndexColor6 = GL_COLOR_ATTACHMENT6,
		GLAttachIndexColor7 = GL_COLOR_ATTACHMENT7,
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