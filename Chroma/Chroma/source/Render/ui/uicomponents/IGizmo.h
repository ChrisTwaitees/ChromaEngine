#ifndef CHROMA_IGIZMO_H
#define CHROMA_IGIZMO_H

#include <shader/Shader.h>
#include <component/UIComponent.h>

namespace Chroma
{
	class IGizmo : public UIComponent
	{
	public : 
		IGizmo();
		virtual ~IGizmo() = default;

		virtual void Init() override;
		virtual void Draw() override;

		virtual void SetTransform(const glm::mat4& newTransform) { m_Transform = newTransform; }
		virtual glm::mat4 GetTransform() const { return m_Transform; }

	protected : 

		// attrs
		Shader m_Shader;
		const char* m_VtxSource{ "resources/shaders/vertexCrossDebug.glsl" };
		const char* m_FragSouce{"resources/shaders/fragCrossDebug.glsl" };
		const char* m_GeomSource{ "resources/shaders/geometryCrossDebug.glsl" };
		unsigned int pointVAO{ 0 }, pointVBO{ 0 };
	protected:
		// transform
		glm::mat4 m_Transform{ 1.0f };
		// point VAO
		void GeneratePointVAO();
		void BindPointVAO();
	};
}


#endif // CHROMA_IGIZMO_H