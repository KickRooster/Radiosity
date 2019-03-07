#include "RLDevice.h"
#include "..\..\Helper\Helper.h"

namespace Core
{
	RLDevice::RLDevice()
		:m_renderMode(RLRenderMode_Normal)
	{
		m_context = OpenRLCreateContext(NULL, ErrorCatcher, NULL);
		OpenRLSetCurrentContext(m_context);
	}

	void RLDevice::Initialize(std::shared_ptr<RLSL> vertexShader, std::shared_ptr<RLSL> rayShader, std::shared_ptr<RLSL> bakeFrameShader, std::shared_ptr<RLSL> pinholeFrameShader)
	{
		m_vertexShader = vertexShader;
		m_vertexShader.lock()->BeginUse();
		m_vertexShader.lock()->Attach(this);

		m_rayShader = rayShader;
		m_rayShader.lock()->BeginUse();
		m_rayShader.lock()->Attach(this);

		m_environmentProgram = std::make_unique<RLProgram>();

		m_environmentProgram->AttachShader(m_vertexShader.lock()->GetRLShader());
		m_environmentProgram->AttachShader(m_rayShader.lock()->GetRLShader());
		m_environmentProgram->Link();

		m_bakeFrameShader = bakeFrameShader;
		m_bakeFrameShader.lock()->BeginUse();
		m_bakeFrameShader.lock()->Attach(this);

		m_bakeProgram = std::make_unique<RLProgram>();

		m_bakeProgram->AttachShader(m_bakeFrameShader.lock()->GetRLShader());
		m_bakeProgram->Link();

		rlGenPrimitives(1, &m_environmentPrimitive);
		rlCheckError();

		rlBindPrimitive(RL_PRIMITIVE, m_environmentPrimitive);
		rlCheckError();

		//rlPrimitiveParameterString(RL_PRIMITIVE, RL_PRIMITIVE_NAME, "Environment");

		rlUseProgram(m_environmentProgram->GetRawData());
		rlCheckError();

		//////////////////////////////////////////////////////////////////////////

		m_pinholeProgram = std::make_unique<RLProgram>();

		m_pinholeFrameShader = pinholeFrameShader;
		
		m_pinholeFrameShader.lock()->BeginUse();
		m_pinholeFrameShader.lock()->Attach(this);

		m_pinholeProgram->AttachShader(m_pinholeFrameShader.lock()->GetRLShader());
		m_pinholeProgram->Link();

		rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
		rlCheckError();

		rlUseProgram(m_pinholeProgram->GetRawData());
		rlCheckError();

		int32 loc = rlGetUniformLocation(m_pinholeProgram->GetRawData(), "environmentPrimitive");
		rlCheckError();
		
		rlUniformp(loc, m_environmentPrimitive);
		rlCheckError();

		rlClearColor(0, 0, 0, 0);
		rlCheckError();

		rlFrameParameter1i(RL_FRAME_RAY_DEPTH_LIMIT, m_maxRayDepth);
		rlCheckError();
	}

	void RLDevice::Clear()
	{
		rlClear(RL_COLOR_BUFFER_BIT);
		rlCheckError();
	}

	void RLDevice::SetViewport(int32 xOffset, int32 yOffset, int32 width, int32 height)
	{
		rlViewport(xOffset, yOffset, width, height);
		rlCheckError();
	}

	void RLDevice::SetTransformMatrix(void * pTransformMatrix)
	{
		rlPrimitiveParameterMatrixf(RL_PRIMITIVE, RL_PRIMITIVE_TRANSFORM_MATRIX, (float*)pTransformMatrix);
		rlCheckError();
	}

	void RLDevice::SetRenderMode(RLRenderMode mode)
	{
		m_renderMode = mode;

		if (m_renderMode == RLRenderMode_Normal)
		{
			rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
			rlCheckError();

			rlUseProgram(m_pinholeProgram->GetRawData());
			rlCheckError();

			int32 loc = rlGetUniformLocation(m_pinholeProgram->GetRawData(), "environmentPrimitive");
			rlCheckError();

			rlUniformp(loc, m_environmentPrimitive);
			rlCheckError();
		}
		else if (m_renderMode == RLRenderMode_Bake)
		{
			rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
			rlCheckError();

			rlUseProgram(m_bakeProgram->GetRawData());
			rlCheckError();

			int32 loc = rlGetUniformLocation(m_bakeProgram->GetRawData(), "environmentPrimitive");
			rlCheckError();

			rlUniformp(loc, m_environmentPrimitive);
			rlCheckError();
		}
	}

	void RLDevice::UpdateFrameParams(const void * pMatrix, float fovY)
	{
		rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
		rlCheckError();

		rlUseProgram(m_pinholeProgram->GetRawData());
		rlCheckError();

		m_pinholeProgram->SetUniform1f("fieldOfView", fovY);
		m_pinholeProgram->SetUniformMatrix4f("camera2World", static_cast<const float*>(pMatrix));
	}

	void RLDevice::UpdateFrameParams(const void * pRandomTextureMatrix, const Vector3 * pPosition, const Vector3 * pForwad, const Vector3 * pUp, const Vector3 * pRight, float fovY)
	{
		rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
		rlCheckError();

		rlUseProgram(m_pinholeProgram->GetRawData());
		rlCheckError();

		m_pinholeProgram->SetUniformMatrix4f("randomTextureMatrix", static_cast<const float*>(pRandomTextureMatrix));
		m_pinholeProgram->SetUniform3f("cameraPosition", pPosition);
		m_pinholeProgram->SetUniform3f("forward", pForwad);
		m_pinholeProgram->SetUniform3f("up", pUp);
		m_pinholeProgram->SetUniform3f("right", pRight);
		m_pinholeProgram->SetUniform1f("fieldOfView", fovY);
	}

	void RLDevice::ExecuteCommands()
	{
		rlRenderFrame();
		rlCheckError();
	}

	void RLDevice::UpdateAhead()
	{
		m_pinholeProgram->DetachShader(m_pinholeFrameShader.lock()->GetRLShader());
		m_environmentProgram->DetachShader(m_vertexShader.lock()->GetRLShader());
		m_environmentProgram->DetachShader(m_rayShader.lock()->GetRLShader());
	}

	void RLDevice::UpdateBehind()
	{
		m_pinholeProgram->AttachShader(m_pinholeFrameShader.lock()->GetRLShader());
		m_pinholeProgram->Link();

		m_environmentProgram->AttachShader(m_vertexShader.lock()->GetRLShader());
		m_environmentProgram->AttachShader(m_rayShader.lock()->GetRLShader());
		m_environmentProgram->Link();
	}

	void RLDevice::SetPositionTexture(RLTexture2D * pTexture)
	{
		m_bakeProgram->SetTexture("positionTexture", pTexture->GetRawTexture());
	}

	void RLDevice::SetNormalTexture(RLTexture2D * pTexture)
	{
		m_bakeProgram->SetTexture("normalTexture", pTexture->GetRawTexture());
	}

	void RLDevice::SetJitterTexture(RLTexture2D * pTexture)
	{
		m_pinholeProgram->SetTexture("jitterTexture", pTexture->GetRawTexture());
	}

	RLDevice::~RLDevice()
	{
		OpenRLDestroyContext(m_context);
	}
}