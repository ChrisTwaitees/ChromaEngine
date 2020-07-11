#include "Math.h"

#include <core/Application.h>
#include <scene/Scene.h>

namespace Chroma
{
	glm::vec3 Math::ScreenToWorldRay(const glm::vec2& cursorCoordinates)
	{
		//calculate NDC Coordinates
		float x = (2.0f * cursorCoordinates.x) / Application::Get().GetWindow().GetWidth() - 1.0f;
		float y = 1.0f - (2.0f * cursorCoordinates.y) / Application::Get().GetWindow().GetHeight();
		float z = 1.0f;

		// Normalized Device Coordinates
		glm::vec3 ray_nds(x, y, z);

		// Homogenous Clip Coordinates
		glm::vec4 ray_clip(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

		// Eye Coordinates
		// we bring the homogenous clip coordinates into eye space using the 
		// inverse of the projection matrix
		glm::vec4 ray_eye = glm::inverse(Scene::GetRenderCamera()->GetProjectionMatrix()) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

		// World Coordinates
		// we bring the view/eye coordinates into world space with the 
		// inverse of the view matrix
		glm::vec3 ray_world = glm::vec3((glm::inverse(Scene::GetRenderCamera()->GetViewMatrix()) * ray_eye));
		ray_world = glm::normalize(ray_world);

		return ray_world;
	}
}