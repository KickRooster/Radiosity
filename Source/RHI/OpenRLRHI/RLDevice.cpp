#include "RLDevice.h"
#include "..\..\Helper\Helper.h"

namespace Core
{
	RLDevice::RLDevice()
	{
		m_context = OpenRLCreateContext(NULL, ErrorCatcher, NULL);
		OpenRLSetCurrentContext(m_context);
	}

	void RLDevice::Initialize(std::shared_ptr<RLSL> vertexShader, std::shared_ptr<RLSL> rayShader, std::shared_ptr<RLSL> bakeFrameShader)
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

		rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
		rlCheckError();

		//////////////////////////////////////////////////////////////////////////
		rlClearColor(0, 0, 0, 1.0);
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

	void RLDevice::SetTransformMatrix(RLprogram Program, void * pTransformMatrix)
	{
		RLint Loc = rlGetUniformLocation(Program, "object2World");
		rlCheckError();

		rlUniformMatrix4fv(Loc, 1, RL_FALSE, static_cast<float *>(pTransformMatrix));
		rlCheckError();
		
		//rlPrimitiveParameterMatrixf(RL_PRIMITIVE, RL_PRIMITIVE_TRANSFORM_MATRIX, (float*)pTransformMatrix);
		//rlCheckError();
	}

	void RLDevice::BeginBake(RLTexture2D* pPositionTexture, RLTexture2D* pNormalTexture, RLTexture2D* pHammersleyTexture, RLBuffer* ShootingPrimitiveBuffer)
	{
		rlBindPrimitive(RL_PRIMITIVE, RL_NULL_PRIMITIVE);
		rlCheckError();

		rlUseProgram(m_bakeProgram->GetRawData());
		rlCheckError();

		m_bakeProgram->SetTexture("positionTexture", pPositionTexture->GetRawTexture());
		rlCheckError();

		m_bakeProgram->SetTexture("normalTexture", pNormalTexture->GetRawTexture());
		rlCheckError();

		m_bakeProgram->SetTexture("hammersleyTexture", pHammersleyTexture->GetRawTexture());
		rlCheckError();

		int32 loc = rlGetUniformBlockIndex(m_bakeProgram->GetRawData(), ShootingPrimitiveBuffer->name);
		rlCheckError();

		rlUniformBlockBuffer(loc, ShootingPrimitiveBuffer->GetRawData());
		rlCheckError();
		
		loc = rlGetUniformLocation(m_bakeProgram->GetRawData(), "environmentPrimitive");
		rlCheckError();

		rlUniformp(loc, m_environmentPrimitive);
		rlCheckError();
	}

	void RLDevice::ExecuteCommands()
	{
		rlRenderFrame();
		rlCheckError();
	}

	void RLDevice::UpdateAhead()
	{
		m_environmentProgram->DetachShader(m_vertexShader.lock()->GetRLShader());
		m_environmentProgram->DetachShader(m_rayShader.lock()->GetRLShader());
	}

	void RLDevice::UpdateBehind()
	{
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
		//m_pinholeProgram->SetTexture("jitterTexture", pTexture->GetRawTexture());
	}

	RLDevice::~RLDevice()
	{
		OpenRLDestroyContext(m_context);
	}
}