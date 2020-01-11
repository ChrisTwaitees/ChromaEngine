#ifndef _CHROMA_CHARACTERCONTROLLER_COMPONENT_H_
#define _CHROMA_CHARACTERCONTROLLER_COMPONENT_H_

// thirdparty
#include <glm/glm.hpp>

// chroma
#include <common/CoreCommon.h>
#include <component/IComponent.h>
#include <camera/ICameraController.h>
#include <physics/PhysicsEngine.h>
#include <math/Math.h>


class CharacterControllerComponent : public IComponent
{
protected:
	// camera controller
	ICameraController* m_CameraController{ nullptr };
	glm::vec3 m_CamPosition{ glm::vec3(10.0) };
	glm::vec3 m_CamDirection{ CHROMA_RIGHT };
	glm::vec3 m_CamUp{ CHROMA_UP };

	// transform
	glm::vec3 m_Position{ 0.0f };
	glm::quat m_Rotation{ glm::quat() };
	glm::vec3 m_Scale{ 1.0 };

	glm::mat4 m_Transform{ 1.0 };

	// functions
	virtual void ProcessInput();
	void CalculateTransform();

public:
	// Functions
	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	virtual inline void SetCustomCameraController(ICameraController*& newCameraController) { m_CameraController = newCameraController; }

	inline virtual glm::vec3& GetPlayerPosition() { return m_Position; };
	inline virtual glm::vec3& GetCamPosition() { return m_CamPosition; };

	CharacterControllerComponent();
	virtual ~CharacterControllerComponent();
};

#endif