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

	public:
		enum Mode {Translation, Rotation, Scale};
		inline Mode GetMode() const { return m_Mode; }
		inline void SetMode(const Mode& newMode) { m_Mode = newMode; }
		
		inline void SetActive(const bool& isActive) { m_Active = isActive; }
		inline bool SetActive() const { return m_Active; }

		inline void SetSize(const float& newSize) {	m_Size = newSize; }
		inline float GetSize() { return m_Size; }

	public: 
		inline void SetTransform(const glm::mat4& newTransform) { m_Transform = newTransform; }
		inline glm::mat4 GetTransform() const { return m_Transform; }

	private : 
		glm::mat4 m_Transform{ 1.0f };
		Mode m_Mode{ Translation };
		bool m_Active{false};
		float m_Size{ 1.0f };
	private:
		// translation
		const char* m_TransFragSource{ "resources/shaders/fragGizmo.glsl" };
		const char* m_TransVtxSource{ "resources/shaders/vertexGizmo.glsl" };
		const char* m_TransGeomSource{ "resources/shaders/geometryGizmoTranslation.glsl" };
		Shader m_TranslateShader{ m_TransFragSource, m_TransVtxSource, m_TransGeomSource };

		// rotation
		const char* m_RotFragSource{ "resources/shaders/fragCrossDebug.glsl" };
		const char* m_RotVtxSource{ "resources/shaders/vertexCrossDebug.glsl" };
		const char* m_RotGeomSource{ "resources/shaders/geometryCrossDebug.glsl" };
		Shader m_RotationShader{ m_RotFragSource, m_RotVtxSource, m_RotGeomSource };

		// scale
		const char* m_ScaleFragSource{ "resources/shaders/fragCrossDebug.glsl" };
		const char* m_ScaleVtxSource{ "resources/shaders/vertexCrossDebug.glsl" };
		const char* m_ScaleGeomSource{ "resources/shaders/geometryCrossDebug.glsl" };
		Shader m_ScaleShader{ m_ScaleFragSource, m_ScaleVtxSource, m_ScaleGeomSource };

		// pointVAO
		virtual void GeneratePointBuffers() override;
		unsigned int m_PointArraySize{ 3 };
		virtual void BindDrawVAO() override;
	private:
		// collisions
		std::vector<RigidBody*> m_RigidBodies;
		void GenerateColliders();

	};
}

#endif // CHROMA_TRANSLATEGIZMO_H