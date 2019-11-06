#ifndef _CHROMA_MOUSEPICKER_
#define _CHROMA_MOUSEPICKER_

#include <glm/glm.hpp>


#include "screenmanager/ChromaScreenManagerConfig.h"
#include "camera/Camera.h"

class Camera;

class MousePicker
{
private:
	glm::vec3 mouseToWorld(Camera* cam, float mouseX, float mouseY);
	glm::vec3 lastWorldPos{glm::vec3(0)};

public:
	glm::vec3 getMouseToWorld(Camera* cam, float mouseX, float mouseY);

	void onClick(Camera* cam, float mouse, float mouseY);

	MousePicker() {};
	~MousePicker() {};
};

#endif