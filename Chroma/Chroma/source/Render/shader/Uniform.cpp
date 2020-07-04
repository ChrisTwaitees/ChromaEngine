#include "Uniform.h"

namespace Chroma
{
	void Uniform::SetBool(const std::string& name, bool value, unsigned int const& shaderID) 
	{
		glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
	}

	void Uniform::SetInt(const std::string& name, int value, unsigned int const& shaderID)
	{
		glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
	}

	void Uniform::SetFloat(const std::string& name, float value, unsigned int const& shaderID)
	{
		glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
	}

	void Uniform::setVec2(const std::string& name, glm::vec2 value, unsigned int const& shaderID)
	{
		glUniform2f(glGetUniformLocation(shaderID, name.c_str()), value.x, value.y);
	}

	void Uniform::setVec3(const std::string& name, glm::vec3 value, unsigned int const& shaderID)
	{
		glUniform3f(glGetUniformLocation(shaderID, name.c_str()), value.x, value.y, value.z);
	}

	void Uniform::SetVec4(const std::string& name, glm::vec4 const& value, unsigned int const& shaderID)
	{
		glUniform4f(glGetUniformLocation(shaderID, name.c_str()), value.x, value.y, value.z, value.w);
	}

	void Uniform::SetMat4(const std::string& name, glm::mat4 value, unsigned int const& shaderID)
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}


	void UniformArray::SetUniforms(unsigned int shaderID)
	{
		for (std::pair<std::string, int> uniform : m_IntUniforms)
			SetInt(uniform.first, uniform.second, shaderID);

		for (std::pair<std::string, float> uniform : m_FloatUniforms)
			SetFloat(uniform.first, uniform.second, shaderID);

		for (std::pair<std::string, glm::vec2> uniform : m_Vec2Uniforms)
			setVec2(uniform.first, uniform.second, shaderID);

		for (std::pair<std::string, glm::vec3> uniform : m_Vec3Uniforms)
			setVec3(uniform.first, uniform.second, shaderID);

		for (std::pair<std::string, glm::mat4> uniform : m_Mat4Uniforms)
			SetMat4(uniform.first, uniform.second, shaderID);

		for (std::pair<std::string, unsigned int> uniform : m_UIntUniforms)
			SetInt(uniform.first, uniform.second, shaderID);

	}

	int UniformArray::size()
	{
		return m_IntUniforms.size() + m_FloatUniforms.size() + m_Vec2Uniforms.size() + m_Vec3Uniforms.size() + m_Mat4Uniforms.size();
	}
}
