#ifndef _CHROMA_MAYACAM_
#define _CHROMA_MAYACAM_

// chroma
#include <camera/ICameraController.h>

class MayaCameraController : public ICameraController
{
	void processMouseInput(ChromaInput* const& input) override {};
	void processKeyboardInput(ChromaInput* const& input) override {};

public:
	MayaCameraController() {};
	~MayaCameraController() {};
};

#endif