#include "Camera.h"
#include <core/Application.h>
#include <event/CameraEvent.h>
#include <camera/MayaCameraController.h>
#include <camera/FlyCameraController.h>

namespace Chroma
{
	void Camera::Init()
	{
		m_CamAspect = (float)Chroma::Application::Get().GetWindow().GetWidth() / (float)Chroma::Application::Get().GetWindow().GetHeight();
		UpdateProjectionMatrix();
		UpdateViewMatrix();
		m_Dirty = true;
	}

	void Camera::OnEvent(Event& e)
	{
		if (Chroma::Application::Get().GetWindow().GetCursorEnabled())
		{
			m_CameraController->OnEvent(e, m_CameraPosition, m_CameraDirection, m_CameraUp);
		}
		else
			m_FirstMouse = true;
	}

	void Camera::UpdateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_CameraPosition, m_CameraPosition + glm::normalize(m_CameraDirection), m_CameraUp);
		m_ViewProjMatrix = m_ProjectionMatrix * m_ViewMatrix;
		m_Dirty = true;
	}

	void Camera::UpdateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_CamFOV), m_CamAspect, m_CamNear, m_CamFar);
		m_ViewProjMatrix = m_ProjectionMatrix * m_ViewMatrix;
		m_Dirty = true;
	}

	Camera::Camera()
	{
		m_CameraController = std::make_shared<MayaCameraController>();
		Init();
	}

	Camera::Camera(glm::vec3 cameraPos_val, glm::vec3 cameraTarget_val) : m_CameraPosition{ cameraPos_val }
	{
		m_CameraDirection = glm::normalize(m_CameraPosition - cameraTarget_val);
		glm::vec3 camRight = glm::normalize(glm::cross(CHROMA_UP, m_CameraDirection));
		m_CameraUp = glm::cross(m_CameraDirection, camRight);
		Init();
	}

	inline void Camera::SetCameraMode(CameraMode newMode)
	{
		if (newMode != m_CameraMode)
		{
			m_CameraMode = newMode;

			switch (newMode)
			{
			case CameraMode::Maya :
			{
				m_CameraController = std::make_shared<MayaCameraController>();
				break;
			}
			case CameraMode::FlyCam:
			{
				m_CameraController = std::make_shared<FlyCameraController>();
				break;
			}
			default:
			{
				CHROMA_ASSERT(false, "CameraMode parsed not supported!");
			}
			}
		}
	}

	void Camera::OnCameraEvent(Event& e)
	{
		CHROMA_INFO("Received CameraEvent from Subscribed Camera");

		if (e.GetCategoryFlags() == EventCategory::EventCategoryCamera)
		{
			CHROMA_INFO("Received CameraEvent from Subscribed Camera");
			m_Dirty = true;
		}

		// camera moved 
		if (e.GetEventType() == EventType::CameraMoved)
		{
			m_Dirty = true;
			UpdateViewMatrix();
		}

		// Calculate Camera Velocity
		m_CamVelocity = m_CameraPosition - m_PrevCameraPosition;

		//Debug
		//CHROMA_INFO("Camera Velocity : {}, {}, {}", m_CamVelocity.x, m_CamVelocity.y, m_CamVelocity.z);
		//CHROMA_INFO("Camera Changed this Frame : {}", m_Dirty);
		// Reset Camera Update
		m_Dirty = false;

		// TODO: Implement CameraMoved event
		// Capture last frame's Position
		m_PrevCameraPosition = m_CameraPosition;

	}


}
