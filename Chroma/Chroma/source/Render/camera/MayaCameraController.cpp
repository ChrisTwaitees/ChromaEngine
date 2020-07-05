#include "MayaCameraController.h"
#include <input/Input.h>
#include <event/MouseEvent.h>
#include <event/KeyEvent.h>
#include <event/CameraEvent.h>

namespace Chroma
{


	void MayaCameraController::CalculateUpandDir(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
	{
		camDir = glm::normalize(pivot - camPos);
		camRight = glm::normalize(glm::cross(CHROMA_UP, camDir));
		camUp = glm::normalize(glm::cross(camDir, camRight));
	}

	void MayaCameraController::OnEvent(Event& e, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
	{
		CHROMA_INFO("Maya Camera Controller Receiving Event : {0}", e);

		CalculateUpandDir(camPos, camDir, camUp);

		// mouse m_Offset
		mouseXYOffset = Chroma::Input::GetMouseXYOffset();
		mouseXYOffset *= glm::vec2(mouseSensitivity);


		CameraMovedEvent camMovedEvent(camPos);
		SendEventToApplication(camMovedEvent);


		//// mouse event
		//if (e.GetEventType() == EventType::MouseButtonPressed)
		//{
		//	if(static_cast)
		//}

		//// panning
		//if (Chroma::Input::IsPressed(Chroma::Input::LEFT_ALT) && Chroma::Input::IsPressed(Chroma::Input::MIDDLE_MOUSE))
		//{
		//	// maya's camera seems to be pivot-based 
		//	pivot += camRight * mouseXYOffset.x;
		//	pivot += -camUp * mouseXYOffset.y;

		//	camPos += camRight * mouseXYOffset.x;
		//	camPos += -camUp * mouseXYOffset.y;
		//}

		//// rotation
		//else if (Chroma::Input::IsPressed(Chroma::Input::LEFT_ALT) && Chroma::Input::IsPressed(Chroma::Input::LEFT_MOUSE))
		//{
		//	camPos += camRight * mouseXYOffset.x;
		//	camPos += -camUp * mouseXYOffset.y;
		//}

		//// zoom
		//else if (Chroma::Input::IsPressed(Chroma::Input::LEFT_ALT) && Chroma::Input::IsPressed(Chroma::Input::RIGHT_MOUSE))
		//{
		//	camPos += camDir * mouseXYOffset.x;
		//	camPos += camDir * -mouseXYOffset.y;
		//}

		//calc up and dir
		CalculateUpandDir(camPos, camDir, camUp);
	}
}
