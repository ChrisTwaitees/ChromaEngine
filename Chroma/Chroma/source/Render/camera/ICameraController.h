#ifndef _CHROMA_CAMERACONTROLLER_
#define _CHROMA_CAMERACONTROLLER_

// thirdparty
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// chroma 
#include <common/CoreCommon.h>
#include <math/Math.h>


struct ICameraController
{
	glm::vec3 m_CamPos{ glm::vec3(0.0) };
	glm::vec3 m_CamDir{ CHROMA_RIGHT };
	glm::vec3 m_CamUp{ CHROMA_UP };


	virtual void ProcessInput(glm::vec3& camPos, glm::vec3& camDir, glm::vec3& camUp) = 0;
};

#endif