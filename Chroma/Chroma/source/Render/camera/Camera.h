#ifndef _CHROMA_CAMERA_H_
#define _CHROMA_CAMERA_H_

//stl
#include <iostream>

// thirdparty
#include <glm/glm.hpp>

// chroma
#include <math/ChromaMath.h>
#include <screenmanager/ChromaScreenManagerConfig.h>
#include <camera/FlyCameraController.h>
#include <camera/MayaCameraController.h>
#include <input/ChromaInput.h>

class ChromaInput;
enum CameraMode { FLYCAM, MAYA};

class Camera
{
	// Camera Attrs
	glm::vec3 cameraPosition{ glm::vec3(0.0, 4.0, 20.0) };
	glm::vec3 cameraUp{ CHROMA_UP };
	glm::vec3 cameraDirection{ CHROMA_BACK };

	// Modes
	CameraMode CamMode = MAYA;
	
	// Camera Contollers
	ICameraController* MayaCamController{ new MayaCameraController() };
	ICameraController* FlyCamController{ new FlyCameraController() };

	// Camera Attributes
	float CAM_FOV{ 45.0f };
	float CAM_ASPECT{ SCREEN_WIDTH / SCREEN_HEIGHT };
	float CAM_NEAR{ 0.1f };
	float CAM_FAR{ 100.0f };
	bool firstMouse{ true };

	// Matrices
	glm::mat4 viewMatrix{ glm::mat4(1) };
	glm::mat4 projectionMatrix{ glm::mat4(1) };
	void updateViewMatrix();
	void updateProjectionMatrix();

	// functions
	void initialize();

public:
	// process
	void processInput(ChromaInput* const& input);

	// getters
	inline glm::vec3 getPosition() const { return cameraPosition; };
	inline glm::vec3 getDirection() const { return cameraDirection; };
	// matrices
	inline glm::mat4 getProjectionMatrix() const { return projectionMatrix; };
	inline glm::mat4 getViewMatrix() const { return viewMatrix; };
	inline glm::mat4 getViewProjMatrix() const { return projectionMatrix * viewMatrix; };

	// setters
	inline void setFOV(float newFOV) { CAM_FOV = newFOV; updateProjectionMatrix(); };
	inline void setASPECT(float newASPECT) { CAM_ASPECT = newASPECT; updateProjectionMatrix(); };
	inline void setNEAR(float newNEAR) { CAM_NEAR = newNEAR; updateProjectionMatrix(); };
	inline void setFAR(float newFAR) { CAM_FAR = newFAR; updateProjectionMatrix(); };
	inline void setCameraMode(CameraMode newMode) { CamMode = newMode; };

	// constructors
	Camera();
	Camera(glm::vec3 camerPos, glm::vec3 cameraTarget);
	~Camera() {};
};

#endif