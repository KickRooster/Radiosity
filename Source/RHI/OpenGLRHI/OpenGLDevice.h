#pragma once
#include "BaseGL.h"
#include "GLBuffer.h"

#include MAP_INCLUDE_PATH

namespace Core
{
	class OpenGLDevice //: public IDevice
	{
	private:
		int32 m_maxUniformBufferBindingCount;
		int32 m_bindingCursor;

		ctd::map<GLShaderDataAlias, int32> m_aliasBindingIDMap;
		ctd::map<GLShaderDataAlias, std::unique_ptr<GLBuffer>> m_aliasBufferMap;

		void queryLimts();
		void registerShaderGlobalData(GLShaderDataAlias alias, int32 dataSize);

	public:
		OpenGLDevice();
		void UploadGlobalShaderData(GLShaderDataAlias alias, int32 dataSize, void * pData);
		void DrawElements(GLTopology topology, int32 count, GLDataType dataType, const void * pIndices);
		void BeforeRender(int32 width, int32 height, float a = 1.0f);
		void AfterRender();
		void BeforePostprocess(int32 xOffset, int32 yOffset, int32 width, int32 height);
		void AfterPostprocess();
		void SetViewport(int32 x, int32 y, int32 width, int32 height);
		void Clear();
		void EnableCullFace();
		void DisableCullFace();
		~OpenGLDevice();
	};

}