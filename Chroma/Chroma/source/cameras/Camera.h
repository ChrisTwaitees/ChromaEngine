#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <math.h>
#include <iostream>
class Camera
{
protected:
	// build reference frame
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	void rebuildView();

	// camera aim
	float lastX, lastY, yaw, pitch;
	float mouseSensitivity{0.05f};
	bool firstMouse;
	float maxPitch{ 90.0f }, maxYaw{90.0f};

	// loot at attrs
	glm::vec3 cameraPos{ 0.0f, 0.0f, -6.0f };
	glm::vec3 cameraTarget{ 0.0f, 0.0f, 0.0f };
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

public:
	// input to vx shader
	glm::mat4 view;

	// speed
	float walkSpeed{ 3.0f };
	float sprintSpeed{ 6.0f };
	float cameraSpeed{0.05f};
	enum Speed {WALK, SPRINT};
	void processMouseInput(const double& xpos, const double& ypos );

	// movement
	enum Direction {FORWARD, BACK, LEFT, RIGHT};
	void move(Direction dir = Direction::FORWARD, Speed speed = Speed::WALK);

	// getters
	glm::vec3 get_position() { return cameraPos; };
	glm::vec3 get_direction() { return cameraDirection; };

	// constructors
	Camera();
	Camera(glm::vec3 camerPos, glm::vec3 cameraTarget);
	~Camera();
};

#endif