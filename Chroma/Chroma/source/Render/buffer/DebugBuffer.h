#ifndef _CHROMA_DEBUG_RENDERER_
#define _CHROMA_DEBUG_RENDERER_

//common
#include <common/PrecompiledHeader.h>
// chroma
#include <buffer/IFramebuffer.h>
#include <camera/Camera.h>
#include <model/Vertex.h>
#include <model/SpherePrimitive.h>
#include <model/BoxPrimitive.h>

class Scene;

namespace Chroma
{

	struct LineShape
	{
		glm::vec3 start{ 0.0 };
		glm::vec3 end{ 0.0 };
		glm::vec3 color{ 0.0 };
	};

	struct GridShape
	{
		glm::vec3 color{ 0.5 };
		glm::mat4 transform{ 1.0 };
		float size{ 10.0 };
	};


	struct BoxShape
	{
		glm::mat4 transform{ 1.0 };
		glm::vec3 bbox_min{ 0.0 }, bbox_max{ 0.0 };
		glm::vec3 color{ 1.0 };
	};

	struct SphereShape
	{
		glm::mat4 transform{ 1.0 };
		float m_Radius{ 1.0 };
		glm::vec3 color{ 1.0 };
	};

	struct JointShape
	{
		glm::mat4 transform{ 1.0 };
		glm::vec3 jointPos{ 0.0 };
		glm::vec3 childPos{ 0.0 };
		glm::vec3 color{ 1.0 };
		float size{ 1.0 };
	};

	struct CoordinatesShape
	{
		glm::mat4 transform{ 1.0 };
		float size{ 1.0 };
	};

	struct CrossShape
	{
		glm::mat4 transform{ 1.0 };
		glm::vec3 color{ 0.0 };
		float size{ 1.0 };
	};

	class DebugBuffer : public IFramebuffer
	{
	public:
		// Grid
		void DrawGrid(float const& size = 10.0, glm::vec3 const& color = glm::vec3(0.5));
		void DrawOverlayGrid(float const& size = 10.0, glm::vec3 const& color = glm::vec3(0.5));

		// Coordinates
		void DrawCoordinates(const glm::mat4& transform, const float& size = 1.0);
		void DrawOverlayCoordinates(const glm::mat4& transform, const float& size = 1.0);

		// Line
		void DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);
		void DrawOverlayLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);

		// Box
		void DrawBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::vec3& color);
		void DrawBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::mat4& trans, const glm::vec3& color);
		void DrawOverlayBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::vec3& color);

		// Sphere
		void DrawSphere(const glm::vec3& center, const float& m_Radius, const glm::vec3& color);
		void DrawOverlaySphere(const glm::vec3& center, const float& m_Radius, const glm::vec3& color);

		// Sphere
		void DrawCross(const glm::vec3& worldPos, const float& size, const glm::vec3& color);
		void DrawOverlayCross(const glm::vec3& worldPos, const float& size, const glm::vec3& color);

		// Joint
		void DrawOverlayJoint(const glm::vec3& originPosition, const glm::vec3 childPosition, const glm::mat4 jointTransform, const float& size = 1.0, const glm::vec3 & color = glm::vec3(1.0));

		// Skeletons
		void ToggleDrawSkeletons();
		void DrawSceneSkeletons();

		// Constraints
		void ToggleDrawSkeletonConstraints();
		void DrawSceneSkeletonConstraints();

		// Buffer Funcs
		void ClearColorAndDepth() override;
		void Draw() override;
		void DrawOverlay(IFramebuffer* drawOver);

		DebugBuffer(IFramebuffer* const& prevFrameBuffer) : m_PostFXBuffer(prevFrameBuffer) { Init(); };

	private:
		// debugSkeletons
		bool m_DebugSkeletons{false};
		bool m_DebugSkeletonConstraints{ false };

		// shapes
		std::vector<LineShape> m_Lines;
		std::vector<LineShape> m_OverlayLines;

		std::vector<BoxShape> m_Boxes;
		std::vector<BoxShape> m_OverlayBoxes;

 		std::vector<SphereShape> m_Spheres;
		std::vector<SphereShape> m_OverlaySpheres;

		std::vector<CrossShape> m_Crosses;
		std::vector<CrossShape> m_OverlayCrosses;

		std::vector<JointShape> m_OverlayJoints;

		std::vector<CoordinatesShape> m_Coordinates;
		std::vector<CoordinatesShape> m_OverlayCoordinates;

		std::vector<GridShape> m_Grids;
		std::vector<GridShape> m_OverlayGrids;

		// attrs
		unsigned int pointVAO, pointVBO;

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
		// sphere geometry shader
		const char* sphereVtxSource{ "resources/shaders/vertexSphereDebug.glsl" };
		const char* sphereGeomSource{ "resources/shaders/geometrySphereDebug.glsl" };
		Shader m_SphereShader{ FragSource, sphereVtxSource, sphereGeomSource };
		// joint geometry shader
		const char* jointFragSource{ "resources/shaders/fragJointDebug.glsl" };
		const char* jointVtxSource{ "resources/shaders/vertexJointDebug.glsl" };
		const char* jointGeomSource{ "resources/shaders/geometryJointDebug.glsl" };
		Shader m_JointShader{ jointFragSource, jointVtxSource, jointGeomSource };
		// coordinates geometry shader
		const char* coordinatesFragSource{ "resources/shaders/fragCoordinatesDebug.glsl" };
		const char* coordinatesVtxSource{ "resources/shaders/vertexCoordinatesDebug.glsl" };
		const char* coordinatesGeomSource{ "resources/shaders/geometryCoordinatesDebug.glsl" };
		Shader m_CoordinatesShader{ coordinatesFragSource, coordinatesVtxSource, coordinatesGeomSource };
		// cross geometry shader
		const char* crossFragSource{ "resources/shaders/fragCrossDebug.glsl" };
		const char* crossVtxSource{ "resources/shaders/vertexCrossDebug.glsl" };
		const char* crossGeomSource{ "resources/shaders/geometryCrossDebug.glsl" };
		Shader m_CrossShader{ crossFragSource, crossVtxSource, crossGeomSource };
		// grid geometry shader
		const char* gridVtxSource{ "resources/shaders/vertexGridDebug.glsl" };
		const char* gridGeomSource{ "resources/shaders/geometryGridDebug.glsl" };
		Shader m_GridShader{ FragSource, gridVtxSource, gridGeomSource };

		// point VAO
		void GeneratePointVAO();

		// previous framebuffer to call from
		IFramebuffer* m_PostFXBuffer;

		// functions
		void DrawShapes();
		void DrawOverlayShapes();
		void DrawDepthCulledShapes();
		void RenderLine(LineShape const& line);
		void RenderSphere(SphereShape const& sphere);
		void RenderBox(BoxShape const& box);
		void RenderCross(CrossShape const& cross);
		void RenderJoint(JointShape const& joint);
		void RenderCoordinate(CoordinatesShape const& coordinate);
		void RenderGrid(GridShape const& grid);

		void BindPointVAO();
		// blitting depth buffer before rendering
		void Init() override;
	};
}
#endif