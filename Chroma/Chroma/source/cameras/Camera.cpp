#include "Camera.h"

void Camera::rebuildView()
{
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
}

void Camera::processMouseInput(const double& xpos, const double& ypos)
{
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

	// clamp pitch and yaw
	//pitch = glm::clamp(pitch, -maxPitch, maxPitch);
	//yaw = glm::clamp(yaw, -maxYaw, maxYaw);

	// calculating cameraDirection using trig 
	glm::vec3 direction;
	direction.x = std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw));
	direction.y = std::sin(glm::radians(pitch));
	direction.z = std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw));
	cameraDirection = glm::normalize(direction);
	rebuildView();
}

void Camera::move(Direction dir, Speed speed)
{
	switch (speed)
	{
	case WALK:
		cameraSpeed *= walkSpeed;
		break;
	case SPRINT:
		cameraSpeed *= sprintSpeed;
	}

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
	rebuildView();
}

Camera::Camera(glm::vec3 cameraPos_val, glm::vec3 cameraTarget_val) : cameraPos{ cameraPos_val }, cameraTarget{ cameraTarget_val }
{
	cameraDirection = glm::normalize(cameraPos - cameraTarget);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	rebuildView();
}




Camera::~Camera()
{
}
