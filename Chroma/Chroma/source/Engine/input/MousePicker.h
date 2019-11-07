#ifndef _CHROMA_MOUSEPICKER_
#define _CHROMA_MOUSEPICKER_

#include <glm/glm.hpp>


#include "screenmanager/ChromaScreenManagerConfig.h"
#include "camera/Camera.h"

class Camera;

class MousePicker
{
private:
	static glm::vec3 mouseToWorld(Camera* cam, float mouseX, float mouseY);

public:
	static glm::vec3 getMouseToWorld(Camera* cam, float mouseX, float mouseY);

	MousePicker() {};
	~MousePicker() {};
};

#endif