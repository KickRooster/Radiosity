#include "Camera.h"
#include "..\RHI\OpenGLRHI\OpenGLDevice.h"
#include "..\3rdParty\DearUI\imgui.h"
#include "..\Helper\Helper.h"

using namespace Core;

namespace Core
{
	Camera::Camera():
		m_perspectiveProjectioneMatrix(Matrix4x4Identify),
		m_viewMatrix(Matrix4x4Identify),
		m_viewMatrixInv(Matrix4x4Identify),
		m_viewPerspectiveProjectionMatrix(Matrix4x4Identify),
		m_leftMouseClicked(False),
		m_rightMouseClicked(False),
		m_pUniformBuffer(std::make_unique<GLBuffer>()),
		m_initialized(False),
		m_rightMousePosPrev(Vector2(0, 0)),
		m_yaw(-90.0f),
		m_pitch(0),
		OrthoParams()
	{
		
	}

	Vector3 Camera::GetWorldUp()
	{
		Vector3 LookDir = Normalize(lookAtDir);
		
		if (Dot(LookDir, Up) > 0.85f)
		{
			return Forward;
		}
		else if (Dot(LookDir, Up) < -0.85f)
		{
			return  -Forward;
		}
		else
		{
			return Up;
		}
	}
	
	void Camera::UpdateViewMatrix()
	{
		m_viewMatrix = LookAt(position, position + lookAtDir, Up);
		m_viewMatrixInv = Inverse(m_viewMatrix);
	}

	void Camera::UpdateMatrix()
	{
		m_viewMatrix = LookAt(position, position + lookAtDir, Up);
		m_viewMatrixInv = Inverse(m_viewMatrix);
		
		m_perspectiveProjectioneMatrix = Perspective(fovY, ascept, zNear, zFar);
		m_viewPerspectiveProjectionMatrix = m_perspectiveProjectioneMatrix * m_viewMatrix;

		m_orthoProjectionMatrix = Ortho(OrthoParams.Left, OrthoParams.Right, OrthoParams.Bottom, OrthoParams.Top, OrthoParams.ZNear, OrthoParams.ZFar);
	}
	
	void Camera::UpdatePerspectiveProjectionMatrix()
	{
		m_perspectiveProjectioneMatrix = Perspective(fovY, ascept, zNear, zFar);
	}

	void Camera::Tick(float deltaTime, const InputState & inputState)
	{
		if (inputState.inlvaid)
			return;

		float moveSpeed = 30.0f * deltaTime * inputState.zPos;
		float rotateSpeed = 3.0f * deltaTime;
		
		if (inputState.leftShiftDown)
		{
			moveSpeed *= 2.0f;
			rotateSpeed *= 2.0f;
		}

		if (inputState.wDown)
			position += GetZAxis(m_viewMatrix) * -moveSpeed;

		if (inputState.sDown)
			position += GetZAxis(m_viewMatrix) * moveSpeed;

		if (inputState.aDown)
			position += GetXAxis(m_viewMatrix) * -moveSpeed;
		
		if (inputState.dDown)
			position += GetXAxis(m_viewMatrix) * moveSpeed;

		if (inputState.qDown)
			position += GetYAxis(m_viewMatrix) * -moveSpeed;

		if (inputState.eDown)
			position += GetYAxis(m_viewMatrix) * moveSpeed;

		if (inputState.rightBuffonDown)
		{
			if (!m_rightMouseClicked)
			{
				m_rightMousePosPrev.x = static_cast<float>(inputState.xPos);// ImGui::GetMousePos().x;
				m_rightMousePosPrev.y = static_cast<float>(inputState.yPos);// ImGui::GetMousePos().y;

				m_rightMouseClicked = True;
			}
			else
			{
				Vector2 rightMousePos = Vector2(
					inputState.xPos,//::GetMousePos().x,
					inputState.yPos);// ImGui::GetMousePos().y);

				float xDelta = rightMousePos.x - m_rightMousePosPrev.x;
				float yDelta = rightMousePos.y - m_rightMousePosPrev.y;

				m_viewMatrix = Rotate(m_viewMatrix, 0.2f * xDelta * Deg2Rad, GetYAxis(m_viewMatrix));
				m_viewMatrix = Rotate(m_viewMatrix, 0.2f * yDelta * Deg2Rad, GetXAxis(m_viewMatrix));

				lookAtDir = -GetZAxis(m_viewMatrix);

				m_rightMousePosPrev.x = rightMousePos.x;
				m_rightMousePosPrev.y = rightMousePos.y;
			}
		}
		else
		{
			m_rightMouseClicked = False;
		}

		m_viewMatrix = LookAt(position, position + lookAtDir, Up);
		
		m_viewMatrixInv = Inverse(m_viewMatrix);
		m_viewPerspectiveProjectionMatrix = m_perspectiveProjectioneMatrix * m_viewMatrix;
	}

	void Camera::UpdataGLParam(OpenGLDevice * pDevice)
	{
		CameraUniformData CameraUniformData;
		CameraUniformData.ViewMatrix = m_viewMatrix;
		CameraUniformData.PerspectiveProjectionMatrix = m_perspectiveProjectioneMatrix;
		CameraUniformData.OrthoProjectionMatrix = m_orthoProjectionMatrix;
		CameraUniformData.Position = Vector4(position.x, position.y, position.z, 1.0f);
		CameraUniformData.NearFar.x = zNear;
		CameraUniformData.NearFar.y = zFar;
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_CameraUniformData, sizeof(CameraUniformData), &CameraUniformData);
	}

	Matrix4x4 * Camera::GetViewMatrix()
	{
		return &m_viewMatrix;
	}

	Matrix4x4 * Camera::GetPerspectiveProjectionMatrix()
	{
		return &m_perspectiveProjectioneMatrix;
	}

	Matrix4x4* Camera::GetViewPerspcetiveProjectionMatrix()
	{
		return &m_viewPerspectiveProjectionMatrix;
	}


	Vector3 Camera::GetForward() const
	{
		return -GetZAxis(m_viewMatrix);
	}

	Vector3 Camera::GetUp() const
	{
		return GetYAxis(m_viewMatrix);
	}

	Vector3 Camera::GetRight() const
	{
		return GetXAxis(m_viewMatrix);
	}

	Camera::~Camera()
	{

	}
}