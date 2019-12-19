#include "FlyCameraController.h"
#include <input/Input.h>

void FlyCameraController::ProcessMouseInput(Input* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	// fetch current xpos and ypos
	glm::vec2 mouseXYOffset = input->GetMouseXYOffset();

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

void FlyCameraController::processKeyboardInput(Input* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	// MOVEMENT
	if (input->IsPressed(Input::LEFT_SHIFT))
		cameraSpeed = fastSpeed * input->GetDeltaTime();
	else
		cameraSpeed = slowSpeed * input->GetDeltaTime();

	if (input->IsPressed(Input::W))
		move(FORWARD, camPos, camDir, camUp);

	if (input->IsPressed(Input::S))
		move(BACK, camPos, camDir, camUp);

	if (input->IsPressed(Input::D))
		move(RIGHT, camPos, camDir, camUp);

	if (input->IsPressed(Input::A))
		move(LEFT, camPos, camDir, camUp);

	if (input->IsPressed(Input::Q))
		move(UP, camPos, camDir, camUp);

	if (input->IsPressed(Input::E))
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

void FlyCameraController::ProcessInput(Input* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	ProcessMouseInput(input, camPos, camDir, camUp);
	processKeyboardInput(input, camPos, camDir, camUp);
}
