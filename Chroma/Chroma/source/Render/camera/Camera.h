#ifndef _CHROMA_CAMERA_H_
#define _CHROMA_CAMERA_H_

//stl
#include <iostream>

// thirdparty
#include <glm/glm.hpp>

// chroma
#include <math/ChromaMath.h>
#include <ChromaConfig.h>
#include <camera/FlyCameraController.h>
#include <camera/MayaCameraController.h>
#include <input/ChromaInput.h>

class ChromaInput;
enum CameraMode { FlyCam, Maya};

class Camera
{
	// Camera Attrs
	glm::vec3 m_CameraPosition{ glm::vec3(0.0, 4.0, 20.0) };
	glm::vec3 m_CameraUp{ CHROMA_UP };
	glm::vec3 m_CameraDirection{ CHROMA_BACK };

	// Modes
	CameraMode m_CamMode = Maya;
	
	// Camera Contollers
	ICameraController* m_MayaCamController{ new MayaCameraController() };
	ICameraController* m_FlyCamController{ new FlyCameraController() };

	// Camera Attributes
	float m_CamFOV{ 45.0f };
	float m_CamAspect{ SCREEN_WIDTH / SCREEN_HEIGHT };
	float m_CamNear{ 0.1f };
	float m_CamFar{ 100.0f };
	bool m_FirstMouse{ true };

	// Matrices
	glm::mat4 m_ViewMatrix{ glm::mat4(1) };
	glm::mat4 m_ProjectionMatrix{ glm::mat4(1) };
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

	// Functions
	void Initialize();

public:
	// Process
	void ProcessInput(ChromaInput* const& input);

	// Accessors
	inline glm::vec3 GetPosition() const { return m_CameraPosition; };
	inline glm::vec3 GetDirection() const { return m_CameraDirection; };
	// matrices
	inline glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; };
	inline glm::mat4 GetViewMatrix() const { return m_ViewMatrix; };
	inline glm::mat4 GetViewProjMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; };
	
	inline void SetFOV(float newFOV) { m_CamFOV = newFOV; UpdateProjectionMatrix(); };
	inline void SetASPECT(float newASPECT) { m_CamAspect = newASPECT; UpdateProjectionMatrix(); };
	inline void SetNEAR(float newNEAR) { m_CamNear = newNEAR; UpdateProjectionMatrix(); };
	inline void SetFAR(float newFAR) { m_CamFar = newFAR; UpdateProjectionMatrix(); };
	inline void SetCameraMode(CameraMode newMode) { m_CamMode = newMode; };

	// Structors
	Camera();
	Camera(glm::vec3 camerPos, glm::vec3 cameraTarget);
	~Camera() {};
};

#endif