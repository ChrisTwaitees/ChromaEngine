#ifndef _CHROMA_FLYCAM_
#define _CHROMA_FLYCAM_

// chroma
#include <camera/ICameraController.h>

class FlyCameraController : public ICameraController
{
	// camera controller interface
	void processMouseInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) override;
	void processKeyboardInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) override;

	// attrs
	// camera aim
	bool firstMouse{ true };
	float yaw{ 0.0f }, pitch{ 0.0f };
	float mouseSensitivity{ 0.05f };
	float maxPitch{ 90.0f }, maxYaw{ 90.0f };

	// camera movement
	enum Direction { FORWARD, BACK, LEFT, RIGHT, UP, DOWN };
	void move(Direction dir, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp);
	float slowSpeed{ 6.0f };
	float fastSpeed{ 12.0f };
	float cameraSpeed{ 0.05f };
	
public:
	void processInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) override;
	FlyCameraController() {};
	~FlyCameraController() {};
};

#endif

