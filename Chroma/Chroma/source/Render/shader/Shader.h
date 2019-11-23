#ifndef _SHADER_H_
#define _SHADER_H_

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Uniform.h"
#include "renderer/ChromaRendererConfig.h"
#include "camera/Camera.h"
#include "light/Light.h"


struct Material {
	glm::vec3 ambient;
	float ambientBrightness;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	float roughness;
};

class Shader
{
private:
	// Functions
	void CheckCompileErrors(GLuint shader, std::string type);
	void CompileAndLink();
	void LoadShaderSource();
	std::string ExpandShaderSource(std::string shaderSourcePath);

	//Attrs
	std::string shaderDir{"resources/shaders/"};
	std::string fragSourcePath, vertexSourcePath, geometrySourcePath;
	std::string fragCode, vertexCode, geometryCode;

	// Uniforms
	Uniform Uniforms{ &ShaderID };

public:
	//program ID
	unsigned int ShaderID;

	// Rendering
	virtual void use();

	// Uniforms
	void setLightingUniforms(std::vector<Light*> Lights, Camera& renderCam);

	template<typename UniformType>
	void addUniform(std::string uniformName, UniformType uniformValue){
		Uniforms.addUniform(uniformName, uniformValue);
	};

	template<typename UniformType>
	void setUniform(std::string uniformName, UniformType uniformValue) {
		Uniforms.setUniform(uniformName, uniformValue);
	};

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setMat4(const std::string& name, glm::mat4 matrix) const;

	// Set Uniforms
	void setUniforms();

	//constructor reads and builds the shader
	Shader(std::string fragmentPath, std::string vertexPath, std::string geometryPath="");
	Shader();
	~Shader();
};


#endif
