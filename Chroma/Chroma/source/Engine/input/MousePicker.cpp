#include "MousePicker.h"

glm::vec3 MousePicker::mouseToWorld(Camera* cam, float mouseX, float mouseY)
{
	//calculate NDC Coordinates
	float x = (2.0f * mouseX) / SCREEN_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / SCREEN_HEIGHT;
	float z = 1.0f;

	// Normalized Device Coordinates
	glm::vec3 ray_nds(x, y, z);

	// Homogenous Clip Coordinates
	glm::vec4 ray_clip(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

	// Eye Coordinates
	// we bring the homogenous clip coordinates into eye space using the 
	// inverse of the projection matrix
	glm::vec4 ray_eye = glm::inverse(cam->projectionMat) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	// World Coordinates
	// we bring the view/eye coordinates into world sapce with the 
	// inverse of the view matrix
	glm::vec3 ray_world = glm::vec3(glm::inverse(cam->viewMat) * ray_eye);
	ray_world = glm::normalize(ray_world);

	// debug
	//std::cout << "world ray : " << ray_world.x << " : " << ray_world.y << " : " << ray_world.z << std::endl;

	return ray_world;
}

glm::vec3 MousePicker::getMouseToWorld(Camera* cam, float mouseX, float mouseY)
{
	return mouseToWorld(cam, mouseX, mouseY);
}

