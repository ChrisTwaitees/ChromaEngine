#ifndef _CHROMA_DEBUG_RENDERER_
#define _CHROMA_DEBUG_RENDERER_

// stl
#include <iostream>
#include <vector>
// thirdparty
#include <glm/glm.hpp>
// chroma
#include <buffer/Framebuffer.h>
#include <camera/Camera.h>
#include <model/Vertex.h>
#include <model/SpherePrimitive.h>
#include <model/BoxPrimitive.h>

class DebugBuffer : public Framebuffer
{
	unsigned int debugVAO, debugVBO;
	std::vector<ChromaVertex> vertices;

	Camera* m_renderCamera;

	const char* lineShaderVtxSource{ "resources/shaders/vertexLineDebug.glsl" };
	const char* lineShaderFragSource{ "resources/shaders/fragLineDebug.glsl" };
	const char* lineShaderGeomSource{ "resources/shaders/geometryLineDebug.glsl" };

	Shader m_debugLineShader{ lineShaderFragSource, lineShaderVtxSource, lineShaderGeomSource };
	Shader m_constantShader{ "resources/shaders/fragConstant.glsl", "resources/shaders/vertexShaderLighting.glsl" };
	BoxPrimitive* m_boxPrimitive{new BoxPrimitive};
	void generateLineVAO();

public:
	void drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);
	void drawCircle(const glm::vec3& center, const float& radius, const glm::vec3& color);

	void Draw() override;

	DebugBuffer(Camera* const& camera) : m_renderCamera(camera) {};
};

#endif