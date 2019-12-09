#ifndef _CHROMA_MAYACAM_
#define _CHROMA_MAYACAM_

// chroma
#include <camera/ICameraController.h>

class MayaCameraController : public ICameraController
{
	void processMouseInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) override;
	void processKeyboardInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) override {};

	// attrs
	const float mouseSensitivity{ 0.1f };
	glm::vec3 pivot{ 0.0 };
	glm::vec3 camRight{ 0.0 };
	glm::vec2 mouseXYOffset{ 0.0 };
	float yaw, pitch;

	void calculateUpandDir(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp);

public:
	void ProcessInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) override;

	MayaCameraController() {};
	~MayaCameraController() {};
};

#endif