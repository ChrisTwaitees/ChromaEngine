#ifndef _CHROMA_CAMERACONTROLLER_
#define _CHROMA_CAMERACONTROLLER_

// thirdparty
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// chroma 
#include <common/CoreCommon.h>


struct ICameraController
{
	virtual void ProcessInput(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) = 0;
};

#endif