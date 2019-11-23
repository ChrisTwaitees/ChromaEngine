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

struct LineShape
{
	glm::vec3 start;
	glm::vec3 end;
	glm::vec3 color;
};

struct BoxShape
{
	glm::mat4 transform{ 1 };
	glm::vec3 bbox_min, bbox_max;
	glm::vec3 color;
};

struct SphereShape
{
	glm::mat4 transform{ 1 };
	float radius;
	glm::vec3 color;
};

class DebugBuffer : public Framebuffer
{
	// shapes
	std::vector<LineShape> m_lines;
	std::vector<BoxShape> m_boxes;
	std::vector<SphereShape> m_spheres;

	// attrs
	unsigned int pointVAO, pointVBO;
	Camera* m_renderCamera;

	// shaders
	const char* FragSource{ "resources/shaders/fragLineDebug.glsl" };
	// line geometry shader
	const char* lineVtxSource{ "resources/shaders/vertexLineDebug.glsl" };
	const char* lineGeomSource{ "resources/shaders/geometryLineDebug.glsl" };
	Shader m_LineShader{ FragSource, lineVtxSource, lineGeomSource };
	// box geometry shader
	const char* boxVtxSource{ "resources/shaders/vertexBoxDebug.glsl" };
	const char* boxGeomSource{ "resources/shaders/geometryBoxDebug.glsl" };
	Shader m_BoxShader{ FragSource, boxVtxSource, boxGeomSource };
	// sphere geometry shder
	const char* sphereVtxSource{ "resources/shaders/vertexSphereDebug.glsl" };
	const char* sphereGeomSource{ "resources/shaders/geometrySphereDebug.glsl" };
	Shader m_SphereShader{ FragSource, sphereVtxSource, sphereGeomSource };
	
	// point VAO
	void generatePointVAO();

	// previous framebuffer to call from
	Framebuffer* m_postFXBuffer;

	// functions
	void drawShapes();
	void renderLine(LineShape line);
	void renderSphere(SphereShape sphere);
	void renderBox(BoxShape box);
	// blitting depth buffer before rendering
	void initialize();
	void attachBuffer();
	void blitPostFXBuffer();

public:

	void drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);
	void drawBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::vec3& color);
	void drawBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::mat4& trans, const glm::vec3& color);
	void drawSphere(const glm::vec3& center, const float& radius, const glm::vec3& color);

	void ClearBuffer() override;
	void Draw() override;

	DebugBuffer(Camera* const& camera, Framebuffer* const& prevFrameBuffer) : m_renderCamera(camera), m_postFXBuffer(prevFrameBuffer) { initialize(); };
};

#endif