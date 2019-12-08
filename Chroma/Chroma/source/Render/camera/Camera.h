#ifndef _CHROMA_CAMERA_H_
#define _CHROMA_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <math.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <screenmanager/ChromaScreenManagerConfig.h>
#include <camera/FlyCameraController.h>
#include <camera/MayaCameraController.h>
#include <input/ChromaInput.h>

class ChromaInput;

class Camera
{

	// Camera Attrs
	glm::vec3 cameraPosition{ 0.0f, 5.0f, -5.0f };
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	glm::vec3 cameraTarget{ 0.0f, 5.0f, 0.0f };
	glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPosition);


	// Modes
	enum Mode { FLYCAM, MAYA};
	Mode CameraMode = FLYCAM;
	
	// Camera Contollers
	ICameraController* MayaCamController{ new MayaCameraController() };
	ICameraController* FlyCamController{ new FlyCameraController() };

	// Camera Attributes
	float CAM_FOV{ 45.0f };
	float CAM_ASPECT{ SCREEN_WIDTH / SCREEN_HEIGHT };
	float CAM_NEAR{ 0.1f };
	float CAM_FAR{ 100.0f };

	void rebuildView();
	
	// First Person
	// build reference frame
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };

	// movement
	enum Direction {FORWARD, BACK, LEFT, RIGHT, UP, DOWN};
	void move(Direction dir = Direction::FORWARD);


	// camera aim
	float lastX, lastY, yaw, pitch;
	float mouseSensitivity{0.05f};
	float maxPitch{ 90.0f }, maxYaw{90.0f};


	// input
	void processMouseInput(ChromaInput* const& input);
	void processKeyboardInput(ChromaInput* const& input);

	// Matrices
	glm::mat4 viewMatrix{ glm::mat4(1) };
	glm::mat4 projectionMatrix{ glm::mat4(1) };
	void updateViewMatrix();
	void updateProjectionMatrix();
	// configure
	bool firstMouse{true};

	// speed
	enum Speed {WALK, SPRINT};
	float walkSpeed{ 6.0f };
	float sprintSpeed{ 12.0f };
	float cameraSpeed{ 0.05f};
	void initialize();

public:
	// process
	void processInput(ChromaInput* const& input);

	// getters
	glm::vec3 getPosition() { return cameraPosition; };
	glm::vec3 getDirection() { return cameraDirection; };
	inline glm::mat4 getProjectionMatrix() const { return projectionMatrix; };
	inline glm::mat4 getViewMatrix() const { return viewMatrix; };
	inline glm::mat4 getViewProjMatrix() const { return projectionMatrix * viewMatrix; };

	// constructors
	Camera() ;
	Camera(glm::vec3 camerPos, glm::vec3 cameraTarget);
	~Camera() {};
};

#endif