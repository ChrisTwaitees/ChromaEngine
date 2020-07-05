#include "ICameraController.h"
#include <core/Application.h>


namespace Chroma
{
	void Chroma::ICameraController::SendEventToApplication(Event& e)
	{
		Application::Get().OnEvent(e);
	}
}

