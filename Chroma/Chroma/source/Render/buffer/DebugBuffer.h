#ifndef _CHROMA_DEBUG_RENDERER_
#define _CHROMA_DEBUG_RENDERER_

// stl
#include <iostream>
// thirdparty
#include <glm/glm.hpp>
// chroma
#include <buffer/Framebuffer.h>
#include <camera/Camera.h>

class DebugBuffer : public Framebuffer
{
	Camera* m_renderCamera;

public:
	void drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);
	void drawCircle(const glm::vec3& center, const float& radius, const glm::vec3& color);

	void Bind() override;
	void Draw() override;

	DebugBuffer(Camera* const& camera) : m_renderCamera(camera) {};
};

#endif