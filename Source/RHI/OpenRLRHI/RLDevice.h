#pragma once
#include "BaseRL.h"
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
		std::weak_ptr<RLSL> m_pinholeFrameShader;
		const int32 m_maxRayDepth = 3;

	private:
		OpenRLContext m_context;
		std::unique_ptr<RLProgram> m_pinholeProgram;
		std::unique_ptr<RLProgram> m_environmentProgram;
		std::unique_ptr<RLProgram> m_bakeProgram;

		RLprimitive m_environmentPrimitive;

		RLRenderMode m_renderMode;

	public:
		RLDevice();
		void Initialize(std::shared_ptr<RLSL> vertexShader, std::shared_ptr<RLSL> rayShader, std::shared_ptr<RLSL> bakeFrameShader, std::shared_ptr<RLSL> pinholeFrameShader);
		void Clear();
		void SetViewport(int32 xOffset, int32 yOffset, int32 width, int32 height);
		void SetTransformMatrix(void * pTransformMatrix);
		void SetRenderMode(RLRenderMode mode);
		void UpdateFrameParams(const void * pMatrix, float fovY);
		void UpdateFrameParams(const void * pRandomTextureMatrix, const Vector3 * pPosition, const Vector3 * pForwad, const Vector3 * pUp, const Vector3 * pRight, float fovY);
		void ExecuteCommands();
		virtual void UpdateAhead() override;
		virtual void UpdateBehind() override;
		void SetPositionTexture(RLTexture2D * pTexture);
		void SetNormalTexture(RLTexture2D * pTexture);
		void SetJitterTexture(RLTexture2D * pTexture);
		~RLDevice();
	};
}