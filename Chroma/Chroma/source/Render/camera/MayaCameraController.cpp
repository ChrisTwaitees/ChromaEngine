#include "MayaCameraController.h"
#include <input/Input.h>


void MayaCameraController::ProcessMouseInput(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	// mouse m_Offset
	mouseXYOffset =  Chroma::Input::GetMouseXYOffset();
	mouseXYOffset *= glm::vec2(mouseSensitivity);

	// panning
	if (Chroma::Input::IsPressed(Chroma::Input::LEFT_ALT) && Chroma::Input::IsPressed(Chroma::Input::MIDDLE_MOUSE))
	{
		// maya's camera seems to be pivot-based 
		pivot += camRight * mouseXYOffset.x;
		pivot += -camUp * mouseXYOffset.y;

		camPos += camRight * mouseXYOffset.x;
		camPos += -camUp * mouseXYOffset.y;
	}

	// rotation
	else if (Chroma::Input::IsPressed(Chroma::Input::LEFT_ALT) && Chroma::Input::IsPressed(Chroma::Input::LEFT_MOUSE))
	{
		camPos += camRight * mouseXYOffset.x;
		camPos += -camUp * mouseXYOffset.y;
	}

	// zoom
	else if (Chroma::Input::IsPressed(Chroma::Input::LEFT_ALT) && Chroma::Input::IsPressed(Chroma::Input::RIGHT_MOUSE))
	{
		camPos += camDir * mouseXYOffset.x;
		camPos += camDir * -mouseXYOffset.y;
	}

	//calc up and dir
	CalculateUpandDir(camPos, camDir, camUp);
}



void MayaCameraController::CalculateUpandDir(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	camDir = glm::normalize(pivot - camPos);
	camRight = glm::normalize(glm::cross(CHROMA_UP, camDir));
	camUp = glm::normalize(glm::cross(camDir, camRight));
}

void MayaCameraController::ProcessInput(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	CalculateUpandDir(camPos, camDir, camUp);
	ProcessMouseInput(camPos, camDir, camUp);
}
