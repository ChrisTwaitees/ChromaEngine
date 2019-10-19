#include "Uniform.h"


void Uniform::updateUniforms()
{
	for (std::pair<std::string, int> uniform : intUniforms.uniforms) {
		setInt(uniform.first, uniform.second);
	}
	for (std::pair<std::string, float> uniform : floatUniforms.uniforms) {
		setFloat(uniform.first, uniform.second);
	}
	for (std::pair<std::string, glm::vec2> uniform : vec2Uniforms.uniforms) {
		setVec2(uniform.first, uniform.second);
	}
	for (std::pair<std::string, glm::vec3> uniform : vec3Uniforms.uniforms) {
		setVec3(uniform.first, uniform.second);
	}
	for (std::pair<std::string, glm::mat4> uniform : mat4Uniforms.uniforms) {
		setMat4(uniform.first, uniform.second);
	}
}


void Uniform::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(*ShaderID, name.c_str()), (int)value);
}

void Uniform::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(*ShaderID, name.c_str()), value);
}

void Uniform::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(*ShaderID, name.c_str()), value);
}

void Uniform::setVec2(const std::string& name, glm::vec2 value) const
{
	glUniform2f(glGetUniformLocation(*ShaderID, name.c_str()), value.x, value.y);
}

void Uniform::setVec3(const std::string& name, glm::vec3 vec3) const
{
	glUniform3f(glGetUniformLocation(*ShaderID, name.c_str()), vec3.x, vec3.y, vec3.z);
}

void Uniform::setMat4(const std::string& name, glm::mat4 matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(*ShaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}