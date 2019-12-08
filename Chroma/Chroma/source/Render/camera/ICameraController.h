#ifndef _CHROMA_CAMERACONTROLLER_
#define _CHROMA_CAMERACONTROLLER_

class ChromaInput;

class ICameraController
{
protected:
	virtual void processMouseInput(ChromaInput* const& input) = 0;
	virtual void processKeyboardInput(ChromaInput* const& input) = 0;

public:
	ICameraController() {};
	virtual ~ICameraController() {};
};

#endif