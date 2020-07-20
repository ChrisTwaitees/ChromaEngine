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

		virtual void OnUpdate() = 0;
		virtual void Draw() = 0;

	protected : 
		// point VAO
		unsigned int m_PointVAO{ 0 }, m_PointVBO{ 0 };
		virtual void GeneratePointBuffers();
		virtual void BindDrawVAO();
	};
}


#endif // CHROMA_IGIZMO_H