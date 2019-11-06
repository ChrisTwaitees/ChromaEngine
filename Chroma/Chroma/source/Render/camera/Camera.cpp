#include "Camera.h"

void Camera::rebuildView()
{
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	viewMat = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
}

void Camera::processInput(ChromaInput*& input)
{
	if (input->getCursorEnabled())
	{
		processMouseInput(input);
		processKeyboardInput(input);
	}
	else
		firstMouse = true;
	
}

void Camera::processMouseInput(ChromaInput*& input)
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

void Camera::processKeyboardInput(ChromaInput*& input)
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
		cameraPos += (cameraDirection * cameraSpeed);
		break;
	case BACK : 
		cameraPos -= (cameraDirection * cameraSpeed);
		break;
	case RIGHT:
		cameraPos -= (cameraRight * cameraSpeed);
		break;
	case LEFT:
		cameraPos += (cameraRight * cameraSpeed);
		break;
	case UP:
		cameraPos += (cameraUp * cameraSpeed);
		break;
	case DOWN:
		cameraPos -= (cameraUp * cameraSpeed);
		break;
	}

	rebuildView();
}

Camera::Camera()
{
	viewMat = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
}

Camera::Camera(glm::vec3 cameraPos_val, glm::vec3 cameraTarget_val) : cameraPos{ cameraPos_val }, cameraTarget{ cameraTarget_val }
{
	cameraDirection = glm::normalize(cameraPos - cameraTarget);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	rebuildView();
}
