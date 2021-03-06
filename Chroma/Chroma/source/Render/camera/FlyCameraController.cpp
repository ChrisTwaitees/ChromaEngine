#include "FlyCameraController.h"
#include <input/Input.h>

namespace Chroma
{
	void FlyCameraController::ProcessMouseInput(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
	{
		// fetch current xpos and ypos
		glm::vec2 mouseXYOffset = Chroma::Input::GetMouseXYOffset();

		// applying sensitivity
		mouseXYOffset *= glm::vec2(mouseSensitivity);
	
		// adding yaw and pitch
		yaw += mouseXYOffset.x;
		pitch += mouseXYOffset.y;

		// calculating cameraDirection using trig 
		glm::vec3 m_Direction;
		m_Direction.x = std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw));
		m_Direction.y = std::sin(glm::radians(pitch));
		m_Direction.z = std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw));

		// set direction
		camDir = m_Direction;
		glm::vec3 camRight = glm::normalize(glm::cross(CHROMA_UP, camDir));
		camUp = glm::cross(m_Direction, camRight);
	}

	void FlyCameraController::processKeyboardInput(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
	{
		// TODO: Implement Event System Hook up 
		// MOVEMENT
		//if (Chroma::Input::IsPressed(Chroma::Input::LEFT_SHIFT))
		//	cameraSpeed = fastSpeed * (float)Chroma::Time::GetDeltaTime();
		//else
		//	cameraSpeed = slowSpeed * (float)Chroma::Time::GetDeltaTime();

		//if (Chroma::Input::IsPressed(Chroma::Input::W))
		//	move(FORWARD, camPos, camDir, camUp);

		//if (Chroma::Input::IsPressed(Chroma::Input::S))
		//	move(BACK, camPos, camDir, camUp);

		//if (Chroma::Input::IsPressed(Chroma::Input::D))
		//	move(RIGHT, camPos, camDir, camUp);

		//if (Chroma::Input::IsPressed(Chroma::Input::A))
		//	move(LEFT, camPos, camDir, camUp);

		//if (Chroma::Input::IsPressed(Chroma::Input::Q))
		//	move(UP, camPos, camDir, camUp);

		//if (Chroma::Input::IsPressed(Chroma::Input::E))
		//	move(DOWN, camPos, camDir, camUp);
	}

	void FlyCameraController::move(Direction dir, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
	{
		glm::vec3 camRight = glm::normalize(glm::cross(camUp, camDir));

		switch (dir)
		{
		case FORWARD:
			camPos += (camDir * cameraSpeed);
			break;
		case BACK:
			camPos -= (camDir * cameraSpeed);
			break;
		case RIGHT:
			camPos -= (camRight * cameraSpeed);
			break;
		case LEFT:
			camPos += (camRight * cameraSpeed);
			break;
		case UP:
			camPos += (camUp * cameraSpeed);
			break;
		case DOWN:
			camPos -= (camUp * cameraSpeed);
			break;
		}
	}

	void FlyCameraController::OnEvent(Event& e, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
	{
		ProcessMouseInput(camPos, camDir, camUp);
		processKeyboardInput(camPos, camDir, camUp);
	}
}
