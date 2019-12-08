#include "Camera.h"

void Camera::initialize()
{
	updateProjectionMatrix();
	updateViewMatrix();
}

void Camera::processInput(ChromaInput* const& input)
{
	if (input->getCursorEnabled())
	{
		switch (CamMode)
		{
		case(CameraMode::FLYCAM):
		{
			FlyCamController->processInput(input, cameraPosition, cameraDirection, cameraUp);
			updateViewMatrix();
			break;
		}
		case(CameraMode::MAYA):
		{
			MayaCamController->processInput(input, cameraPosition, cameraDirection, cameraUp);
			updateViewMatrix();
			break;
		}
		}
	}
	else
		firstMouse = true;
	
}

void Camera::updateViewMatrix()
{
	viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
}

void Camera::updateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(CAM_FOV), CAM_ASPECT, CAM_NEAR, CAM_FAR);
}

Camera::Camera()
{
	initialize();
}

Camera::Camera(glm::vec3 cameraPos_val, glm::vec3 cameraTarget_val) : cameraPosition{ cameraPos_val }
{
	cameraDirection = glm::normalize(cameraPosition - cameraTarget_val);
	glm::vec3 camRight = glm::normalize(glm::cross(CHROMA_UP, cameraDirection));
	cameraUp = glm::cross(cameraDirection, camRight);
	initialize();
}
