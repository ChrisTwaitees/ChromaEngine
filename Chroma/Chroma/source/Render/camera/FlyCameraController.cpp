#include "FlyCameraController.h"
#include <input/ChromaInput.h>

void FlyCameraController::processMouseInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	// fetch current xpos and ypos
	glm::vec2 mouseXYOffset = input->getMouseXYOffset();

	// applying sensitivity
	mouseXYOffset *= glm::vec2(mouseSensitivity);
	
	// adding yaw and pitch
	yaw += mouseXYOffset.x;
	pitch += mouseXYOffset.y;

	// calculating cameraDirection using trig 
	glm::vec3 direction;
	direction.x = std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw));
	direction.y = std::sin(glm::radians(pitch));
	direction.z = std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw));

	// set direction
	camDir = direction;
	glm::vec3 camRight = glm::normalize(glm::cross(CHROMA_UP, camDir));
	camUp = glm::cross(direction, camRight);
}

void FlyCameraController::processKeyboardInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	// MOVEMENT
	if (input->isPressed(ChromaInput::LEFT_SHIFT))
		cameraSpeed = fastSpeed * input->getDeltaTime();
	else
		cameraSpeed = slowSpeed * input->getDeltaTime();

	if (input->isPressed(ChromaInput::W))
		move(FORWARD, camPos, camDir, camUp);

	if (input->isPressed(ChromaInput::S))
		move(BACK, camPos, camDir, camUp);

	if (input->isPressed(ChromaInput::D))
		move(RIGHT, camPos, camDir, camUp);

	if (input->isPressed(ChromaInput::A))
		move(LEFT, camPos, camDir, camUp);

	if (input->isPressed(ChromaInput::Q))
		move(UP, camPos, camDir, camUp);

	if (input->isPressed(ChromaInput::E))
		move(DOWN, camPos, camDir, camUp);
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

void FlyCameraController::processInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	processMouseInput(input, camPos, camDir, camUp);
	processKeyboardInput(input, camPos, camDir, camUp);
}
