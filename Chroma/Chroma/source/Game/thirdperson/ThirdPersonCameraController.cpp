#include "ThirdPersonCameraController.h"
#include <input/Input.h>

void ThirdPersonCameraController::ProcessInput(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	camPos = m_CamPos;
	camDir = m_CamDir;
	camUp = m_CamUp;
}
