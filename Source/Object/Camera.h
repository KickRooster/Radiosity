#pragma once
#include <BaseInclude.h>
#include <Vector3.h>
#include <MathGlobal.h>
#include <Matrix4x4.h>
#include "..\RHI\OpenGLRHI\GLBuffer.h"
#include "..\RHI\OpenGLRHI\OpenGLDevice.h"
#include "..\Math\Vector2.h"

namespace Core
{
	struct CameraUniformData
	{
		Matrix4x4 ViewMatrix;
		Matrix4x4 ViewProjectionMatrix;
		Vector4 Position;
		Vector4 NearFar;	//	near,far,
	};
	
	class Camera
	{
	private:
		Matrix4x4 m_perspectiveMatrix;
		Matrix4x4 m_viewMatrix;
		Matrix4x4 m_viewMatrixInv;
		Matrix4x4 m_viewProjectionMatrix;
		Matrix4x4 m_randowTextureMatrix;

		Bool m_leftMouseClicked;
		Bool m_rightMouseClicked;

		Vector2 m_rightMousePosPrev;

		std::unique_ptr<GLBuffer> m_pUniformBuffer;

		Bool m_initialized;

		float m_yaw;
		float m_pitch;

	public:
		float zNear;
		float zFar;
		float ascept;
		float fovY;
		Vector3 position;
		Vector3 lookAt;
		Vector3 eularAngle;

		Camera();
		void UpdateMatrix();
		void UpdateProjectionMatrix();
		void Tick(float deltaTime, const InputState & inputState);
		void UpdataGLParam(OpenGLDevice * pDevice);
		Matrix4x4 * GetViewPeojectionMatrix();
		Matrix4x4 * GetViewMatrix();
		Matrix4x4 * GetPeojectionMatrix();
		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetRight() const;
		~Camera();
	};
}