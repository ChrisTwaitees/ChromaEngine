#ifndef _CHROMA_MAYACAM_
#define _CHROMA_MAYACAM_

// chroma
#include <camera/ICameraController.h>

class MayaCameraController : public ICameraController
{
	void processMouseInput(ChromaInput* const& input) override {};
	void processKeyboardInput(ChromaInput* const& input) override {};

public:
	void processInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) override {};

	MayaCameraController() {};
	~MayaCameraController() {};
};

#endif