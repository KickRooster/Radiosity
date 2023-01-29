#pragma once
#include "BaseRL.h"
#include "RLBuffer.h"
#include "RLShader.h"
#include "RLProgram.h"
#include "..\..\Math\Matrix4x4.h"
#include "..\..\DesignPattern\ObserverPattern\Obeserver.h"
#include "..\..\Asset\RLSL.h"
#include "RLTexture2D.h"

namespace Core
{
	enum RLRenderMode
	{
		RLRenderMode_Normal,
		RLRenderMode_Bake,
	};

	class RLDevice : public Obeserver
	{
	private:
		std::weak_ptr<RLSL> m_vertexShader;
		std::weak_ptr<RLSL> m_rayShader;
		std::weak_ptr<RLSL> m_bakeFrameShader;
		const int32 m_maxRayDepth = 1;

	private:
		OpenRLContext m_context;
		std::unique_ptr<RLProgram> m_environmentProgram;
		std::unique_ptr<RLProgram> m_bakeProgram;

		RLprimitive m_environmentPrimitive;

	public:
		RLDevice();
		void Initialize(std::shared_ptr<RLSL> vertexShader, std::shared_ptr<RLSL> rayShader, std::shared_ptr<RLSL> bakeFrameShader);
		void Clear();
		void SetViewport(int32 xOffset, int32 yOffset, int32 width, int32 height);
		void SetTransformMatrix(RLprogram Program, void * pTransformMatrix);
		void BeginBake(RLTexture2D* pPositionTexture1x1, RLTexture2D* pNormalTexture1x1, RLTexture2D* pPositionTexture2x2, RLTexture2D* pNormalTexture2x2, RLBuffer* ShootingPrimitiveBuffer);
		void ExecuteCommands();
		virtual void UpdateAhead() override;
		virtual void UpdateBehind() override;
		void SetPositionTexture(RLTexture2D * pTexture);
		void SetNormalTexture(RLTexture2D * pTexture);
		void SetJitterTexture(RLTexture2D * pTexture);
		~RLDevice();
	};
}