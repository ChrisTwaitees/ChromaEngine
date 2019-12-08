#include "MayaCameraController.h"
#include <input/ChromaInput.h>

void MayaCameraController::processMouseInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	// mouse offset
	mouseXYOffset = input->getMouseXYOffset();
	mouseXYOffset *= glm::vec2(mouseSensitivity);

	// panning
	if (input->isPressed(ChromaInput::LEFT_ALT) && input->isPressed(ChromaInput::MIDDLE_MOUSE))
	{
		// maya's camera seems to be pivot-based 
		pivot += camRight * mouseXYOffset.x;
		pivot += -camUp * mouseXYOffset.y;

		camPos += camRight * mouseXYOffset.x;
		camPos += -camUp * mouseXYOffset.y;
	}

	// rotation
	else if (input->isPressed(ChromaInput::LEFT_ALT) && input->isPressed(ChromaInput::LEFT_MOUSE))
	{
		camPos += camRight * mouseXYOffset.x;
		camPos += -camUp * mouseXYOffset.y;
	}

	// zoom
	else if (input->isPressed(ChromaInput::LEFT_ALT) && input->isPressed(ChromaInput::RIGHT_MOUSE))
	{
		camPos += camDir * mouseXYOffset.x;
		camPos += camDir * -mouseXYOffset.y;
	}

	//calc up and dir
	calculateUpandDir(camPos, camDir, camUp);
}



void MayaCameraController::calculateUpandDir(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	camDir = glm::normalize(pivot - camPos);
	camRight = glm::normalize(glm::cross(CHROMA_UP, camDir));
	camUp = glm::normalize(glm::cross(camDir, camRight));
}

void MayaCameraController::processInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	calculateUpandDir(camPos, camDir, camUp);
	processMouseInput(input, camPos, camDir, camUp);
}
