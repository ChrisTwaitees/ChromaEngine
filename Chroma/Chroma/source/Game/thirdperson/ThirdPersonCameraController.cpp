#include "ThirdPersonCameraController.h"
#include <input/Input.h>

void ThirdPersonCameraController::ProcessInput(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
{
	if (Chroma::Input::IsPressed(Chroma::Input::CROSS))
	{
		m_test = 10.0;
		Chroma::Time::StartTimer(m_test);
		camPos.y += Chroma::Time::GetDeltaTime() * m_Sensitivity;
	}

	//CHROMA_INFO("TIMER : {0}", m_test);



}
