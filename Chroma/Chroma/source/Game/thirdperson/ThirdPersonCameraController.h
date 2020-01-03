#ifndef _CHROMA_THIRDPERSON_CAMERA_CONTROLLER_H_
#define _CHROMA_THIRDPERSON_CAMERA_CONTROLLER_H_


// chroma
#include <camera/ICameraController.h>


class ThirdPersonCameraController : public ICameraController
{
	float m_Sensitivity{ 10.0f };
	float m_test{ 10.0f };

public:
	void ProcessInput(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) override;

	ThirdPersonCameraController() {};
	~ThirdPersonCameraController() {};

};

#endif
