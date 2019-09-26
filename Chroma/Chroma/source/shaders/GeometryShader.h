#ifndef _CHROMA_GEOMETRY_SHADER_
#define _CHROMA_GEOMETRY_SHADER_
#include "../shaders/Shader.h"
class GeometryShader : public Shader
{
public:
	//program ID
	unsigned int id;

	// use/activate the shader;
	void use() override;

	//constructor reads and builds the shader
	GeometryShader();
	GeometryShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~GeometryShader();
};

#endif