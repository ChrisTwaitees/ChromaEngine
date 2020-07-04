#ifndef CHROMA_IGIZMO_H
#define CHROMA_IGIZMO_H

#include <shader/Shader.h>

class IGizmo
{
public : 
	IGizmo();
	virtual void Init();
	virtual void Draw();

protected : 
	// attrs
	unsigned int pointVAO{ 0 }, pointVBO{ 0 };

	// Shader
	const char* m_VtxSource{ "resources/shaders/vertexLineDebug.glsl" };
	const char* m_FragSouce{ "resources/shaders/fragBasic.glsl" };
	const char* m_GeomSource{ "" };
	Shader m_Shader;

	// point VAO
	void GeneratePointVAO();
	void BindPointVAO();
};


#endif // CHROMA_IGIZMO_H