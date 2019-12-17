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

class ChromaScene;

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
	std::vector<LineShape> m_OverlayLines;
	std::vector<BoxShape> m_boxes;
	std::vector<BoxShape> m_OverlayBoxes;
 	std::vector<SphereShape> m_spheres;
	std::vector<SphereShape> m_OverlaySpheres;

	// attrs
	unsigned int pointVAO, pointVBO;
	Camera* m_renderCamera;

	// shaders
	const char* FragSource{ "resources/shaders/fragBasic.glsl" };
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
	void DrawOverlayShapes();
	void DrawDepthCulledShapes();
	void renderLine(LineShape line);
	void renderSphere(SphereShape sphere);
	void renderBox(BoxShape box);
	// blitting depth buffer before rendering
	void Initialize() override;
	void attachBuffer();
	void blitPostFXBuffer();
	void blitDepthPostFXBuffer();
	

public:
	void DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);
	void DrawOverlayLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);

	void DrawBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::vec3& color);
	void DrawBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::mat4& trans, const glm::vec3& color);
	void DrawOverlayBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::vec3& color);

	void DrawSphere(const glm::vec3& center, const float& radius, const glm::vec3& color);
	void DrawOverlaySphere(const glm::vec3& center, const float& radius, const glm::vec3& color);

	void DrawSceneSkeletons(ChromaScene* const &scene);

	void ClearBuffer() override;
	void Draw() override;

	DebugBuffer(Camera* const& camera, Framebuffer* const& prevFrameBuffer) : m_renderCamera(camera), m_postFXBuffer(prevFrameBuffer) { Initialize(); };
};

#endif