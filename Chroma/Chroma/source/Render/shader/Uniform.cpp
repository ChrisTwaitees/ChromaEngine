#include "Uniform.h"


void Uniform::setUniforms()
{
	for (std::pair<std::string, int> uniform : intUniforms.uniforms) {
		SetInt(uniform.first, uniform.second);
	}
	for (std::pair<std::string, float> uniform : floatUniforms.uniforms) {
		SetFloat(uniform.first, uniform.second);
	}
	for (std::pair<std::string, glm::vec2> uniform : vec2Uniforms.uniforms) {
		setVec2(uniform.first, uniform.second);
	}
	for (std::pair<std::string, glm::vec3> uniform : vec3Uniforms.uniforms) {
		setVec3(uniform.first, uniform.second);
	}
	for (std::pair<std::string, glm::mat4> uniform : mat4Uniforms.uniforms) {
		SetMat4(uniform.first, uniform.second);
	}
}


void Uniform::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(*ShaderID, name.c_str()), (int)value);
}

void Uniform::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(*ShaderID, name.c_str()), value);
}

void Uniform::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(*ShaderID, name.c_str()), value);
}

void Uniform::setVec2(const std::string& name, glm::vec2 value) const
{
	glUniform2f(glGetUniformLocation(*ShaderID, name.c_str()), value.x, value.y);
}

void Uniform::setVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3f(glGetUniformLocation(*ShaderID, name.c_str()), value.x, value.y, value.z);
}

void Uniform::SetMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(*ShaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}