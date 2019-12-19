#ifndef _CHROMA_CAMERACONTROLLER_
#define _CHROMA_CAMERACONTROLLER_

// stl
#include <iostream>

// thirdparty
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// chroma 
class Input;

class ICameraController
{
protected:
	virtual void ProcessMouseInput(Input* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) = 0;
	virtual void processKeyboardInput(Input* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) = 0;

public:
	virtual void ProcessInput(Input* const& input, glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) = 0;

	ICameraController() {};
	virtual ~ICameraController() {};
};

#endif