#pragma once
#include "BaseGL.h"
#include "GLBuffer.h"

#include <map>

namespace Core
{
	class OpenGLDevice //: public IDevice
	{
	private:
		int32 m_maxUniformBufferBindingCount;
		int32 m_bindingCursor;

		std::map<GLShaderDataAlias, int32> m_aliasBindingIDMap;
		std::map<GLShaderDataAlias, std::unique_ptr<GLBuffer>> m_aliasBufferMap;

		void queryLimts();
		void registerShaderGlobalData(GLShaderDataAlias alias, int32 dataSize);

	public:
		OpenGLDevice();
		void UploadGlobalShaderData(GLShaderDataAlias alias, int32 dataSize, void * pData);
		void DrawElements(GLTopology topology, int32 count, GLDataType dataType, const void * pIndices);
		void DrawArrays(GLTopology topology, int32 FirstIndex, int32 IndexCount);
		void BeforeRender(int32 width, int32 height, float a = 1.0f);
		void BeginBasePass(int32 width, int32 height);
		void BeginVisibisityPass(int32 width, int32 height);
		void BeginReconstrucionPass(int32 width, int32 height);
		void BeginPickPass(int32 width, int32 height);
		void AfterRender();
		void BeforePostprocess(int32 xOffset, int32 yOffset, int32 width, int32 height);
		void AfterPostprocess();
		void SetViewport(int32 x, int32 y, int32 width, int32 height);
		void SetClearColor(float r, float g, float b, float a);
		void Clear();
		void EnableCullFace();
		void DisableCullFace();
		~OpenGLDevice();
	};

}