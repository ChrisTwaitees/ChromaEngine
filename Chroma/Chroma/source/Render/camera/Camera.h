#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <math.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "screenmanager/ChromaScreenManagerConfig.h"
#include "input/ChromaInput.h"

class Camera
{
protected:
	// build reference frame
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	void rebuildView();

	// camera aim
	float lastX, lastY, yaw, pitch;
	float mouseSensitivity{0.05f};
	float maxPitch{ 90.0f }, maxYaw{90.0f};

	// loot at attrs
	glm::vec3 cameraPos{ 0.0f, 5.0f, -9.0f };
	glm::vec3 cameraTarget{ 0.0f, 9.0f, 0.0f };
	glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

public:
	// input to vx shader
	glm::mat4 viewMat;
	glm::mat4 projectionMat{ glm::perspective(glm::radians(CAM_FOV), CAM_ASPECT, CAM_NEAR, CAM_FAR)};

	// configure
	bool firstMouse{true};

	// input
	void processInput(ChromaInput*& input);
	void processMouseInput(ChromaInput*& input);
	void processKeyboardInput(ChromaInput*& input);

	// speed
	enum Speed {WALK, SPRINT};
	float walkSpeed{ 6.0f };
	float sprintSpeed{ 12.0f };
	float cameraSpeed{ 0.05f};

	// movement
	enum Direction {FORWARD, BACK, LEFT, RIGHT, UP, DOWN};
	void move(Direction dir = Direction::FORWARD);

	// getters
	glm::vec3 get_position() { return cameraPos; };
	glm::vec3 get_direction() { return cameraDirection; };

	// constructors
	Camera();
	Camera(glm::vec3 camerPos, glm::vec3 cameraTarget);
	~Camera();
};

#endif