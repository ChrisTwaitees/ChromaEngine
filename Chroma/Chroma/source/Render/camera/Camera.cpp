#include "Camera.h"

void Camera::rebuildView()
{
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
}

void Camera::processInput(ChromaInput* const& input)
{
	if (input->getCursorEnabled())
	{
		processMouseInput(input);
		processKeyboardInput(input);
	}
	else
		firstMouse = true;
	
}

void Camera::processMouseInput(ChromaInput* const& input)
{
	double xpos = input->getMouseX();
	double ypos = input->getMouseY();

	// ensuring the camera doesn't snap on initial entry
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// calculate the offset between last and current frame
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	// set last to current frame for next cycle
	lastX = xpos;
	lastY = ypos;

	// set speed 
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	// adding yaw and pitch
	yaw += xoffset;
	pitch += yoffset;

	// calculating cameraDirection using trig 
	glm::vec3 direction;
	direction.x = std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw));
	direction.y = std::sin(glm::radians(pitch));
	direction.z = std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw));
	cameraDirection = glm::normalize(direction);
	rebuildView();
}

void Camera::processKeyboardInput(ChromaInput* const& input)
{
	// MOVEMENT
	if (input->isPressed(ChromaInput::LEFT_SHIFT))
		cameraSpeed = sprintSpeed * input->getDeltaTime();
	else
		cameraSpeed = walkSpeed * input->getDeltaTime();

	if (input->isPressed(ChromaInput::W))
		move(FORWARD);

	if (input->isPressed(ChromaInput::S))
		move(BACK);

	if (input->isPressed(ChromaInput::D))
		move(RIGHT);

	if (input->isPressed(ChromaInput::A))
		move(LEFT);

	if (input->isPressed(ChromaInput::Q))
		move(UP);

	if (input->isPressed(ChromaInput::E))
		move(DOWN);
}


void Camera::move(Direction dir)
{
	switch (dir)
	{
	case FORWARD :
		cameraPosition += (cameraDirection * cameraSpeed);
		break;
	case BACK : 
		cameraPosition -= (cameraDirection * cameraSpeed);
		break;
	case RIGHT:
		cameraPosition -= (cameraRight * cameraSpeed);
		break;
	case LEFT:
		cameraPosition += (cameraRight * cameraSpeed);
		break;
	case UP:
		cameraPosition += (cameraUp * cameraSpeed);
		break;
	case DOWN:
		cameraPosition -= (cameraUp * cameraSpeed);
		break;
	}

	rebuildView();
}

Camera::Camera()
{
	viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
}

Camera::Camera(glm::vec3 cameraPos_val, glm::vec3 cameraTarget_val) : cameraPosition{ cameraPos_val }, cameraTarget{ cameraTarget_val }
{
	cameraDirection = glm::normalize(cameraPosition - cameraTarget);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	rebuildView();
}
