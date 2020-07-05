#ifndef CHROMA_CAMERA_H
#define CHROMA_CAMERA_H

// chroma
#include <math/Math.h>

#include <camera/ICameraController.h>

#include <core/Base.h>

#include <event/Event.h>
#include <event/CameraEvent.h>



namespace Chroma
{
	

	class Camera
	{
	public:
		Camera();
		Camera(glm::vec3 camerPos, glm::vec3 cameraTarget);
		~Camera() = default;


		// Events
		enum CameraMode { FlyCam, Maya, Custom };
		void OnEvent(Event& e);
		void OnCameraEvent(Event& e);

		// Accessors
		inline glm::vec3 GetPosition() const { return m_CameraPosition; };
		inline void SetPosition(const glm::vec3& newPosition) { m_CameraPosition = newPosition; UpdateViewMatrix(); }
		inline glm::vec3 GetDirection() const { return m_CameraDirection; };
		inline void SetDirection(const glm::vec3& newDirection) { m_CameraDirection = newDirection; UpdateViewMatrix(); }
		inline glm::vec3 GetVelocity() const { return m_CamVelocity; };
		inline bool GetDirty() const { return m_Dirty; };

		// matrices
		inline glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; };
		inline glm::mat4 GetViewMatrix() const { return m_ViewMatrix; };
		inline glm::mat4 GetViewProjMatrix() const { return m_ViewProjMatrix; };
	
		// frustrum
		inline void SetFOV(float const& newFOV) { m_CamFOV = newFOV; UpdateProjectionMatrix(); };
		inline float GetFOV() { return m_CamFOV; }
		inline void SetAspectRatio(float const& newASPECT) { m_CamAspect = newASPECT; UpdateProjectionMatrix(); }
		inline float GetAspectRatio() { return m_CamAspect; };
		inline void SetNearDist(float const& newNEAR) { m_CamNear = newNEAR; UpdateProjectionMatrix(); };
		inline float GetNearDist() { return m_CamNear; };
		inline void SetFarDist(float const& newFAR) { m_CamFar = newFAR; UpdateProjectionMatrix(); };
		inline float GetFarDist() { return m_CamFar; };


		// camera controller
		void SetCustomCameraController(std::shared_ptr<ICameraController> newCamController) {
			m_CameraController = std::move(newCamController);
			m_Dirty = true;
		}

		inline void SetCameraMode(CameraMode newMode);

	protected:


		// Camera Attrs
		glm::vec3 m_PrevCameraPosition{ glm::vec3(0.0) };
		glm::vec3 m_CameraPosition{ glm::vec3(0.0, 10.0, 30.0) };
		glm::vec3 m_CamVelocity{ glm::vec3(0.0) };

		glm::vec3 m_CameraUp{ CHROMA_UP };
		glm::vec3 m_CameraDirection{ CHROMA_BACK };

		bool m_Dirty{ false };


		// Modes
		CameraMode m_CameraMode = Maya;

		// Camera Contoller
		std::shared_ptr<ICameraController> m_CameraController;

		// Camera Attributes
		float m_CamFOV{ 45.0f };
		float m_CamAspect{ 1920.0f/1080.0f };
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
		void Init();
	};
}

#endif