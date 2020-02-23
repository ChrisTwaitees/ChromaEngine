#include "Camera.h"
#include <input/Input.h>
#include <screen/Screen.h>

void Camera::Initialize()
{
	//m_CamAspect = (float)Chroma::Screen::GetWidthHeight().first / (float)Chroma::Screen::GetWidthHeight().second;
	UpdateProjectionMatrix();
	UpdateViewMatrix();
}

void Camera::ProcessCustomCameraController()
{
	if (m_CustomCameraController != nullptr)
		m_CustomCameraController->ProcessInput(m_CameraPosition, m_CameraDirection, m_CameraUp);
}

void Camera::Update()
{
	if (Chroma::Input::GetCursorEnabled())
	{
		switch (m_CameraMode)
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
		case(CameraMode::Custom):
		{
			ProcessCustomCameraController();
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
	m_ViewProjMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera::UpdateProjectionMatrix()
{
	m_ProjectionMatrix = glm::perspective(glm::radians(m_CamFOV), m_CamAspect, m_CamNear, m_CamFar);
	m_ViewProjMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera::SetCustomCameraController(ICameraController*& newCameraController)
{
	m_CustomCameraController = newCameraController;
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
