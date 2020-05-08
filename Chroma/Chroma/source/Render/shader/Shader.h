#ifndef _CHROMA_SHADER_H_
#define _CHROMA_SHADER_H_

//common
#include <common/PrecompiledHeader.h>
// chroma
#include <ChromaConfig.h>
#include <shader/Uniform.h>
#include <ChromaConfig.h>
#include <camera/Camera.h>
#include <light/Light.h>
#include <core/Core.h>


class Shader
{
public:
	// Cleanup
	void Destroy();

	//program ID
	unsigned int ShaderID{ 0 };

	// Rendering
	void Use() const;

	// Uniforms
	template<typename UniformType>
	void SetUniform(std::string uniformName, UniformType uniformValue) {
		m_Uniforms.SetUniform(uniformName, uniformValue);
	};

	void SetLightingUniforms(Camera const& renderCam);


	//constructor reads and builds the shader
	Shader(std::string fragmentPath, std::string vertexPath, std::string geometryPath="");
	Shader() {};
	~Shader();

private:
	// Functions
	void BindUniformBufferBlockIndices();
	void CheckCompileErrors(GLuint shader, std::string type);
	void CompileAndLink();
	void LoadShaderSource();
	void CleanUp();
	void Replace(std::string& sourceString, std::string const& from, std::string const& to);
	std::string ExpandShaderSource(std::string shaderSourcePath);

	//Attrs
	std::string shaderDir{ "resources/shaders/" };
	std::string fragSourcePath, vertexSourcePath, geometrySourcePath;
	std::string fragCode, vertexCode, geometryCode;

	// Uniforms
	Uniform m_Uniforms{ &ShaderID };
};


#endif
