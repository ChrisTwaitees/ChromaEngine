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
	//program ID
	unsigned int ShaderID;

	// Rendering
	void Use() const;

	// Uniforms
	void SetLightingUniforms(Camera const& renderCam);

	template<typename UniformType>
	void AddUniform(std::string uniformName, UniformType uniformValue){
		m_Uniforms.addUniform(uniformName, uniformValue);
	};

	template<typename UniformType>
	void SetUniform(std::string uniformName, UniformType uniformValue) {
		m_Uniforms.SetUniform(uniformName, uniformValue);
	};

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, glm::vec2 value) const;
	void SetVec3(const std::string& name, glm::vec3 value) const;
	void SetMat4(const std::string& name, glm::mat4 matrix) const;

	// Set Uniforms
	void SetUniforms();
	inline Uniform& GetUniformArray() { return m_Uniforms; }
	inline void SetUniformArray(const Uniform& newUniform) { m_Uniforms = newUniform; }

	//constructor reads and builds the shader
	Shader(std::string fragmentPath, std::string vertexPath, std::string geometryPath="");
	Shader() {};
	~Shader();

private:
	// Functions
	void CheckCompileErrors(GLuint shader, std::string type);
	void CompileAndLink();
	void LoadShaderSource();
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
