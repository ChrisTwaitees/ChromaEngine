#ifndef _CHROMA_FLYCAM_
#define _CHROMA_FLYCAM_

// chroma
#include <camera/ICameraController.h>

class FlyCameraController : public ICameraController
{
	void processMouseInput(ChromaInput* const& input) override {};
	void processKeyboardInput(ChromaInput* const& input) override {};
public:
	FlyCameraController() {};
	~FlyCameraController() {};
};

#endif

