#ifndef _CHROMA_CAMERACONTROLLER_
#define _CHROMA_CAMERACONTROLLER_

// stl
#include <iostream>

// thirdparty
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// chroma 
class ChromaInput;

class ICameraController
{
protected:
	virtual void processMouseInput(ChromaInput* const& input) = 0;
	virtual void processKeyboardInput(ChromaInput* const& input) = 0;

public:
	virtual void processInput(ChromaInput* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) = 0;

	ICameraController() {};
	virtual ~ICameraController() {};
};

#endif