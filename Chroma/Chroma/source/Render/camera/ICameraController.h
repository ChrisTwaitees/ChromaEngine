#ifndef _CHROMA_CAMERACONTROLLER_
#define _CHROMA_CAMERACONTROLLER_

//common
#include <common/PrecompiledHeader.h>

// chroma 
#include <event/Event.h>

namespace Chroma
{
	class Camera;

	class ICameraController
	{
	public:
		friend class Camera;
		virtual void OnEvent(Event& e, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) = 0;

		// transforms
		glm::vec3 m_CamPos{ glm::vec3(0.0) };
		glm::vec3 m_CamDir{ CHROMA_RIGHT };
		glm::vec3 m_CamUp{ CHROMA_UP };

		ICameraController() {};
		virtual ~ICameraController() = default;
	protected:
		void SendEventToApplication(Event& e);
	};
}

#endif