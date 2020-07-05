#ifndef _CHROMA_MAYACAM_
#define _CHROMA_MAYACAM_

// chroma
#include <camera/ICameraController.h>

namespace Chroma
{
	class MayaCameraController : public ICameraController
	{
	public:
		MayaCameraController() {}

		void OnEvent(Event& e, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) override;

	private :
		// attrs
		const float mouseSensitivity{ 0.1f };
		glm::vec3 pivot{ 0.0 };
		glm::vec3 camRight{ 0.0 };
		glm::vec2 mouseXYOffset{ 0.0 };
		float yaw{ 0.0 }, pitch{ 0.0 };

		bool m_AltPressed{ false };
		bool m_MiddleMousePressed{ false };
		void CalculateUpandDir(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp);

	};
}

#endif