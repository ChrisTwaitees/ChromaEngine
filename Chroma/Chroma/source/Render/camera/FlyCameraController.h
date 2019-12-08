#ifndef _CHROMA_FLYCAM_
#define _CHROMA_FLYCAM_

// chroma
#include <camera/ICameraController.h>

class FlyCameraController : public ICameraController
{
	// camera controller interface
	void processMouseInput(ChromaInput* const& input) override;
	void processKeyboardInput(ChromaInput* const& input) override;
	
public:
	void processInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) override;
	FlyCameraController() {};
	~FlyCameraController() {};
};

#endif

