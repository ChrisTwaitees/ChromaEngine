#ifndef CHROMA_TRANSLATEGIZMO_H
#define CHROMA_TRANSLATEGIZMO_H


#include <ui/uicomponents/IGizmo.h>

namespace Chroma
{

	class RigidBody;

	class TransformGizmo :	public IGizmo
	{
	public:
		TransformGizmo();
		~TransformGizmo();

		virtual void Draw() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& e) override;

		bool RayHitCheck(const glm::vec3& startRay, const glm::vec3& endRay);

	public:
		enum Mode {Translation, Rotation, Scale};
		inline Mode GetMode() const { return m_Mode; }
		inline void SetMode(const Mode& newMode) { m_Mode = newMode; }
		
		inline void SetActive(const bool& isActive) { m_Active = isActive; }
		inline bool GetActive() const { return m_Active; }

		inline void SetSize(const float& newSize) {	m_Size = newSize; }
		inline float GetSize() { return m_Size; }

	public: 
		inline void SetTransform(const glm::mat4& newTransform) { m_Transform = newTransform; }
		inline glm::mat4 GetTransform() const { return m_Transform; }

	private : 
		glm::mat4 m_LocalTransform{ 1.0f };
		glm::mat4 m_Transform{ 1.0f };
		Mode m_Mode{ Translation };
		
		bool m_Active{false};
		float m_Size{ 1.0f };

	private:
		enum Axis {X, Y, Z};
		Axis m_XAxis{ X }, m_YAxis{ Y }, m_ZAxis{ Z };
		bool m_X{ false }, m_Y{ false }, m_Z{ false };
		bool m_XHovered{ false }, m_YHovered{ false }, m_ZHovered{ false };
		bool m_AxisSelected{ false };
		void CheckIfHovering();
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	private:
		// shaderShared
		const char* m_FragGizmo{ "resources/shaders/fragGizmo.glsl" };
		const char* m_VertGizmo{ "resources/shaders/vertexGizmo.glsl" };

		// translation
		const char* m_TransGeomSource{ "resources/shaders/geometryGizmoTranslation.glsl" };
		Shader m_TranslateShader{ m_FragGizmo, m_VertGizmo, m_TransGeomSource };

		// rotation
		const char* m_RotGeomSource{ "resources/shaders/geometryGizmoRotation.glsl" };
		Shader m_RotationShader{ m_FragGizmo, m_VertGizmo, m_RotGeomSource };

		// scale
		const char* m_ScaleGeomSource{ "resources/shaders/geometryGizmoScale.glsl" };
		Shader m_ScaleShader{ m_FragGizmo, m_VertGizmo, m_ScaleGeomSource };

		// active axis
		void SetActiveAxisUniforms(Shader& shader);
	private:
		// pointVAO
		virtual void GeneratePointBuffers() override;
		virtual void BindDrawVAO() override;
	private:
		// collisions
		std::vector<RigidBody*> m_RigidBodies;
		void GenerateColliders();

	};
}

#endif // CHRO  MA_TRANSLATEGIZMO_H