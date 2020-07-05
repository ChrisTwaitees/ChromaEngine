#ifndef _CHROMA_FLYCAM_
#define _CHROMA_FLYCAM_

// chroma
#include <camera/ICameraController.h>

namespace Chroma
{
	class FlyCameraController : public ICameraController
	{
	public:
		void OnEvent(Event& e, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) override;
	private:
		// camera controller interface
		void ProcessMouseInput(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp);
		void processKeyboardInput(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp);

		// attrs
		// camera aim
		bool m_FirstMouse{ true };
		float yaw{ 0.0f }, pitch{ 0.0f };
		float mouseSensitivity{ 0.05f };
		float maxPitch{ 90.0f }, maxYaw{ 90.0f };

		// camera movement
		enum Direction { FORWARD, BACK, LEFT, RIGHT, UP, DOWN };
		void move(Direction dir, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp);
		float slowSpeed{ 6.0f };
		float fastSpeed{ 12.0f };
		float cameraSpeed{ 0.05f };
	
	};
}

#endif

