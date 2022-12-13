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
		Matrix4x4 PerspectiveProjectionMatrix;
		Matrix4x4 OrthoProjectionMatrix;
		Vector4 Position;
		Vector4 NearFar;	//	near,far,
	};

	struct ShooterInfo
	{
		Vector3 Position;
		Vector3 Normal;
		Vector3 Energy;
		Vector3 SurfaceArea;
	};

	struct OrthoProjectionParams
	{
		float Left;
		float Right;
		float Bottom;
		float Top;
		float ZNear;
		float ZFar;

		OrthoProjectionParams::OrthoProjectionParams()
		{
			Left = 0;
			Right = 0;
			Bottom = 0;
			Top = 0;
			ZNear = 0;
			ZFar = 0;
		}
	};
	
	class Camera
	{
		Matrix4x4 m_viewMatrix;
		Matrix4x4 m_viewMatrixInv;
		Matrix4x4 m_perspectiveProjectioneMatrix;
		Matrix4x4 m_viewPerspectiveProjectionMatrix;
		Matrix4x4 m_orthoProjectionMatrix;

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
		OrthoProjectionParams OrthoParams;
		
		Vector3 position;
		Vector3 lookAt;
		Vector3 eularAngle;

		Camera();
		void UpdateViewMatrix();
		void UpdateMatrix();
		void UpdatePerspectiveProjectionMatrix();
		void Tick(float deltaTime, const InputState & inputState);
		void UpdataGLParam(OpenGLDevice * pDevice);
		Matrix4x4 * GetViewMatrix();
		Matrix4x4 * GetPerspectivePeojectionMatrix();
		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetRight() const;
		~Camera();
	};
}