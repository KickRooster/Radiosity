#include "Camera.h"
#include "..\RHI\OpenGLRHI\OpenGLDevice.h"
#include "..\3rdParty\DearUI\imgui.h"
#include "..\Helper\Helper.h"

using namespace Core;

namespace Core
{
	Camera::Camera():
		m_perspectiveMatrix(Matrix4x4Identify),
		m_viewMatrix(Matrix4x4Identify),
		m_viewMatrixInv(Matrix4x4Identify),
		m_viewProjectionMatrix(Matrix4x4Identify),
		m_randowTextureMatrix(Matrix4x4Identify),
		m_leftMouseClicked(False),
		m_rightMouseClicked(False),
		m_pUniformBuffer(std::make_unique<GLBuffer>()),
		m_initialized(False),
		m_rightMousePosPrev(Vector2(0, 0)),
		m_yaw(-90.0f),
		m_pitch(0)
	{
		
	}

	void Camera::UpdateMatrix()
	{
		m_perspectiveMatrix = Perspective(fovY, ascept, zNear, zFar);

		m_viewMatrix = LookAt(position, position + lookAt, Up);
		m_viewMatrixInv = Inverse(m_viewMatrix);

		m_viewProjectionMatrix = m_perspectiveMatrix * m_viewMatrix;
	}
	
	void Camera::UpdateProjectionMatrix()
	{
		m_perspectiveMatrix = Perspective(fovY, ascept, zNear, zFar);
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

				lookAt = -GetZAxis(m_viewMatrix);

				//m_yaw += xDelta * rotateSpeed;
				//m_pitch += yDelta * rotateSpeed;
				//
				//if (m_pitch > 89.0f)
				//	m_pitch = 89.0f;
				//if (m_pitch < -89.0f)
				//	m_pitch = -89.0f;
				//
				//lookAt.x = cos(m_yaw * Deg2Rad) * cos(m_pitch * Deg2Rad);
				//lookAt.y = sin(m_pitch * Deg2Rad);
				//lookAt.z = sin(m_yaw * Deg2Rad) * cos(m_pitch * Deg2Rad);
				//lookAt = glm::normalize(lookAt);

				m_rightMousePosPrev.x = rightMousePos.x;
				m_rightMousePosPrev.y = rightMousePos.y;
			}
		}
		else
		{
			m_rightMouseClicked = False;
		}

		m_viewMatrix = LookAt(position, position + lookAt, Up);
		
		m_viewMatrixInv = Inverse(m_viewMatrix);
		m_viewProjectionMatrix = m_perspectiveMatrix * m_viewMatrix;
	}

	void Camera::UpdataGLParam(OpenGLDevice * pDevice)
	{
		CameraUniformData CameraUniformData;
		CameraUniformData.ViewMatrix = m_viewMatrix;
		CameraUniformData.ViewProjectionMatrix = m_viewProjectionMatrix;
		CameraUniformData.Position = Vector4(position.x, position.y, position.z, 1.0f);
		CameraUniformData.NearFar.x = zNear;
		CameraUniformData.NearFar.y = zFar;
		pDevice->UploadGlobalShaderData(GLShaderDataAlias_CameraUniformData, sizeof(CameraUniformData), &CameraUniformData);
	}

	Matrix4x4 * Camera::GetViewPeojectionMatrix()
	{
		return &m_viewProjectionMatrix;
	}

	Core::Matrix4x4 * Camera::GetViewMatrix()
	{
		return &m_viewMatrix;
	}

	Core::Matrix4x4 * Camera::GetPeojectionMatrix()
	{
		return &m_perspectiveMatrix;
	}

	Core::Vector3 Camera::GetForward() const
	{
		return -GetZAxis(m_viewMatrix);
	}

	Core::Vector3 Camera::GetUp() const
	{
		return GetYAxis(m_viewMatrix);
	}

	Core::Vector3 Camera::GetRight() const
	{
		return GetXAxis(m_viewMatrix);
	}

	Camera::~Camera()
	{

	}
}