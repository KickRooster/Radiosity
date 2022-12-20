#include "OpenGLDevice.h"

#include "../../Object/Camera.h"
#include "..\..\Math\MathGlobal.h"
#include "..\..\Math\Vector4.h"
#pragma comment (lib, "glew32.lib") 

namespace Core
{
	void OpenGLDevice::queryLimts()
	{
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &m_maxUniformBufferBindingCount);
	}

	void OpenGLDevice::registerShaderGlobalData(GLShaderDataAlias alias, int32 dataSize)
	{
		m_aliasBindingIDMap[alias] = m_bindingCursor;
		++m_bindingCursor;

		m_aliasBufferMap[alias] = std::make_unique<GLBuffer>();
		m_aliasBufferMap[alias]->Recreate(BufferTarget_UniformBuffer, BufferUsage_DynamicDraw, dataSize);

		assert(m_bindingCursor < m_maxUniformBufferBindingCount);
	}

	OpenGLDevice::OpenGLDevice()
	{
		queryLimts();

		registerShaderGlobalData(GLShaderDataAlias_CameraUniformData, sizeof(CameraUniformData));
		registerShaderGlobalData(GLShaderDataAlias_CubeMatrices, sizeof(CubeMatrices));
		registerShaderGlobalData(GLShaderDataAlias_ShooterInfo, sizeof(ShooterInfo));
		registerShaderGlobalData(GLShaderDataAlias_ObjectMatrices, sizeof(Matrix4x4Identify));
		registerShaderGlobalData(GLShaderDataAlias_ObjectMatricesIT, sizeof(Matrix4x4Identify));
		registerShaderGlobalData(GLShaderDataAlias_LightmapUVParam, sizeof(Vector4Dummy));
		registerShaderGlobalData(GLShaderDataAlias_PostprocessParam, sizeof(Vector4Dummy));
		registerShaderGlobalData(GLShaderDataAlias_LightParam, sizeof(Vector3Dummy));

		glFrontFace(GL_CCW);
		glCheckError();

		glEnable(GL_CULL_FACE);
		glCheckError();

		glCullFace(GL_BACK);
		glCheckError();

		glEnable(GL_DEPTH_TEST);
		glCheckError();

		glDepthFunc(GL_LESS);
		glCheckError();

		glClearColor(0, 0.2f, 0.4f, 1.0f);
		glCheckError();

		// Make sure that OpenGL doesn't automatically clamp our display texture. This is because the raytraced image
		// that will be stored in it is actually an accumulation of every ray that has gone through a given pixel.
		// Therefore each pixel value will quickly move beyond 1.0.
		//glClampColorARB(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
		//glCheckError();
	}

	void OpenGLDevice::UploadGlobalShaderData(GLShaderDataAlias alias, int32 dataSize, void * pData)
	{
		m_aliasBufferMap[alias]->Activate(BufferTarget_UniformBuffer);
		m_aliasBufferMap[alias]->UploadData(BufferTarget_UniformBuffer, 0, dataSize, pData);
		glBindBufferBase(GL_UNIFORM_BUFFER, m_aliasBindingIDMap[alias], m_aliasBufferMap[alias]->GetID());
		glCheckError();
		m_aliasBufferMap[alias]->Inactivate(BufferTarget_UniformBuffer);
	}

	void OpenGLDevice::DrawElements(GLTopology topology, int32 count, GLDataType dataType, const void * pIndices)
	{
		glDrawElements(topology, count, dataType, pIndices);
		glCheckError();
	}

	void OpenGLDevice::DrawArrays(GLTopology topology, int32 FirstIndex, int32 IndexCount)
	{
		glDrawArrays(topology, FirstIndex, IndexCount);
		glCheckError();
	}
	
	void OpenGLDevice::BeforeRender(int32 width, int32 height, float a /* = 1.0f */)
	{
		glClearColor(0, 0.2f, 0.4f, a);
		glCheckError();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();

		glViewport(0, 0, width, height);
		glCheckError();

		glFrontFace(GL_CCW);
		glCheckError();

		glEnable(GL_CULL_FACE);
		glCheckError();

		glCullFace(GL_BACK);
		glCheckError();

		glEnable(GL_DEPTH_TEST);
		glCheckError();

		glDepthFunc(GL_LESS);
		glCheckError();
	}

	void OpenGLDevice::BeginVisibisityPass(int32 width, int32 height)
	{
		glClearColor(0, 0, 0, 1);
		glCheckError();

		glClearDepth(1.0);
		glCheckError();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();

		glViewport(0, 0, width, height);
		glCheckError();

		glFrontFace(GL_CCW);
		glCheckError();

		glEnable(GL_CULL_FACE);
		glCheckError();

		glCullFace(GL_BACK);
		glCheckError();

		glEnable(GL_DEPTH_TEST);
		glCheckError();

		glDepthFunc(GL_LESS);
		glCheckError();
	}

	void OpenGLDevice::BeginReconstrucionPass(int32 width, int32 height)
	{
		glClearColor(0, 0, 0, 1);
		glCheckError();

		glClearDepth(1.0);
		glCheckError();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();
		
		glViewport(0, 0, width, height);
		glCheckError();
	}

	void OpenGLDevice::BeginViewCubeMapPass(int32 width, int32 height)
	{
		glClearColor(0, 0, 0, 1.0f);
		glCheckError();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();

		glViewport(0, 0, width, height);
		glCheckError();

		glFrontFace(GL_CCW);
		glCheckError();

		glEnable(GL_CULL_FACE);
		glCheckError();

		glCullFace(GL_BACK);
		glCheckError();

		glEnable(GL_DEPTH_TEST);
		glCheckError();

		glDepthFunc(GL_LESS);
		glCheckError();
	}
	
	void OpenGLDevice::AfterRender()
	{

	}

	void OpenGLDevice::BeforePostprocess(int32 xOffset, int32 yOffset, int32 width, int32 height)
	{
		glClearColor(0, 0, 0, 1.0f);
		glCheckError();
		
		glClear(GL_COLOR_BUFFER_BIT);
		glCheckError();
		
		glViewport(xOffset, yOffset, width, height);
		glCheckError();
		
		glFrontFace(GL_CCW);
		glCheckError();
		
		glEnable(GL_CULL_FACE);
		glCheckError();
		
		glCullFace(GL_BACK);
		glCheckError();
		
		glDisable(GL_DEPTH_TEST);
		glCheckError();
	}

	void OpenGLDevice::AfterPostprocess()
	{
		glEnable(GL_DEPTH_TEST);
		glCheckError();
	}

	void OpenGLDevice::SetViewport(int32 x, int32 y, int32 width, int32 height)
	{
		glViewport(x, y, width, height);
		glCheckError();
	}

	void OpenGLDevice::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glCheckError();
	}
	
	void OpenGLDevice::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();
	}

	OpenGLDevice::~OpenGLDevice()
	{

	}

	void OpenGLDevice::EnableCullFace()
	{
		glEnable(GL_CULL_FACE);
		glCheckError();
	}

	void OpenGLDevice::DisableCullFace()
	{
		glDisable(GL_CULL_FACE);
		glCheckError();
	}
}