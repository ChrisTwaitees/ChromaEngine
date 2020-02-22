#ifndef _CHROMA_CAMERA_H_
#define _CHROMA_CAMERA_H_

//common
#include <common/PrecompiledHeader.h>

// chroma
#include <math/Math.h>
#include <ChromaConfig.h>
#include <camera/FlyCameraController.h>
#include <camera/MayaCameraController.h>

enum CameraMode { FlyCam, Maya, Custom};

class Camera
{
	// Camera Attrs
	glm::vec3 m_CameraPosition{ glm::vec3(0.0, 10.0, 30.0) };
	glm::vec3 m_CameraUp{ CHROMA_UP };
	glm::vec3 m_CameraDirection{ CHROMA_BACK };

	// Modes
	CameraMode m_CameraMode = Maya;
	
	// Camera Contollers
	ICameraController* m_MayaCamController{ new MayaCameraController() };
	ICameraController* m_FlyCamController{ new FlyCameraController() };
	ICameraController* m_CustomCameraController{ nullptr };

	// Camera Attributes
	float m_CamFOV{ 45.0f };
	float m_CamAspect{ SCREEN_WIDTH / SCREEN_HEIGHT };
	float m_CamNear{ 0.1f };
	float m_CamFar{ 200.0f };
	bool m_FirstMouse{ true };

	// Matrices
	glm::mat4 m_ViewMatrix{ 1.0 };
	glm::mat4 m_ProjectionMatrix{ 1.0 };
	glm::mat4 m_ViewProjMatrix{ 1.0 };

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

	// Functions
	void Initialize();
	void ProcessCustomCameraController();

public:
	// Process
	void Update();

	// Accessors
	inline glm::vec3 GetPosition() const { return m_CameraPosition; };
	inline glm::vec3 GetDirection() const { return m_CameraDirection; };
	// matrices
	inline glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; };
	inline glm::mat4 GetViewMatrix() const { return m_ViewMatrix; };
	inline glm::mat4 GetViewProjMatrix() const { return m_ViewProjMatrix; };
	
	inline void SetFOV(float const& newFOV) { m_CamFOV = newFOV; UpdateProjectionMatrix(); };
	inline void SetASPECT(float const& newASPECT) { m_CamAspect = newASPECT; UpdateProjectionMatrix(); };
	inline void SetNEAR(float const& newNEAR) { m_CamNear = newNEAR; UpdateProjectionMatrix(); };
	inline void SetFAR(float const& newFAR) { m_CamFar = newFAR; UpdateProjectionMatrix(); };
	inline void SetCameraMode(CameraMode newMode) { m_CameraMode = newMode; };


	void SetCustomCameraController(ICameraController*& newCameraController);

	// Structors
	Camera();
	Camera(glm::vec3 camerPos, glm::vec3 cameraTarget);
	~Camera() {};
};

#endif