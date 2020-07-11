#include "MayaCameraController.h"
#include <input/Input.h>
#include <event/MouseEvent.h>
#include <event/KeyEvent.h>
#include <event/CameraEvent.h>

namespace Chroma
{


	void MayaCameraController::CalculateUpandDir(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
	{
		
		glm::vec3 newCamDir = glm::normalize(pivot - camPos);
		if (newCamDir != (glm::vec3(-1.0f) * camDir))
			camDir = newCamDir;

		camRight = glm::normalize(glm::cross(CHROMA_UP, camDir));
		camUp = glm::normalize(glm::cross(camDir, camRight));
	}

	void MayaCameraController::OnEvent(Event& e, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp)
	{
		if (e.GetCategoryFlags() == EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryMouse)
		{
			if (e.GetEventType() == EventType::MouseMoved)
			{
				bool cameraMoved = false;
				// update mouse coords if mouse moved
				mouseXYOffset = Chroma::Input::GetMouseXYOffset();
				mouseXYOffset *= glm::vec2(mouseSensitivity);


				// ensure alt is pressed
				if (Input::IsPressed(KeyCode::LeftAlt))
				{
					// panning
					if (Input::IsPressed(MouseCode::ButtonMiddle))
					{
						// maya's camera seems to be pivot-based 
						pivot += -camRight * mouseXYOffset.x;
						pivot += -camUp * mouseXYOffset.y;

						camPos += -camRight * mouseXYOffset.x;
						camPos += -camUp * mouseXYOffset.y;
						// markDirty
						cameraMoved = true;
					}

					// rotation
					else if (Input::IsPressed(MouseCode::ButtonLeft))
					{
						camPos += -camRight * mouseXYOffset.x;
						camPos += -camUp * mouseXYOffset.y;
						// markDirty
						cameraMoved = true;
					}

					// zoom
					else if (Input::IsPressed(MouseCode::ButtonRight))
					{
						// move camera forward						
						camPos += camDir * -mouseXYOffset.x;
						camPos += camDir * -mouseXYOffset.y;

						// if camera very close to pivot, shift pivot forward
						if (glm::distance(camPos, pivot) < 0.25f)
							pivot += camDir * 0.5f;


						// markDirty
						cameraMoved = true;
					}
				}

				// if cameraMoved send event and update camera transforms
				if (cameraMoved)
				{
					CalculateUpandDir(camPos, camDir, camUp);
					CameraMovedEvent camMovedEvent(camPos);
					SendEventToApplication(camMovedEvent);
				}
			}
		}
	}
}
