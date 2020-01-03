#include "Camera.h"
#include <input/Input.h>

void Camera::Initialize()
{
	UpdateProjectionMatrix();
	UpdateViewMatrix();
}

void Camera::ProcessInput()
{
	if (Chroma::Input::GetCursorEnabled())
	{
		switch (m_CamMode)
		{
		case(CameraMode::FlyCam):
		{
			m_FlyCamController->ProcessInput(m_CameraPosition, m_CameraDirection, m_CameraUp);
			UpdateViewMatrix();
			break;
		}
		case(CameraMode::Maya):
		{
			m_MayaCamController->ProcessInput(m_CameraPosition, m_CameraDirection, m_CameraUp);
			UpdateViewMatrix();
			break;
		}
		}
	}
	else
		m_FirstMouse = true;
	
}

void Camera::UpdateViewMatrix()
{
	m_ViewMatrix = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraDirection, m_CameraUp);
}

void Camera::UpdateProjectionMatrix()
{
	m_ProjectionMatrix = glm::perspective(glm::radians(m_CamFOV), m_CamAspect, m_CamNear, m_CamFar);
}

Camera::Camera()
{
	Initialize();
}

Camera::Camera(glm::vec3 cameraPos_val, glm::vec3 cameraTarget_val) : m_CameraPosition{ cameraPos_val }
{
	m_CameraDirection = glm::normalize(m_CameraPosition - cameraTarget_val);
	glm::vec3 camRight = glm::normalize(glm::cross(CHROMA_UP, m_CameraDirection));
	m_CameraUp = glm::cross(m_CameraDirection, camRight);
	Initialize();
}
