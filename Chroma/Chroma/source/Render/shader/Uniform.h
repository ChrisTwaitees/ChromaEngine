#ifndef _CHROMA_UNIFORM_
#define _CHROMA_UNIFORM_

//common
#include <common/PrecompiledHeader.h>
#include <core/Core.h>


class Uniform
{
public:

	template<typename T>
	void SetUniform(std::string name, T uniformValue) {
		CHROMA_ERROR("Unsupported Uniform Type!");
	};

	template<>
	void SetUniform<int>(std::string uniformName, int uniformValue) {
		SetInt(uniformName, uniformValue, *ShaderID);
	};

	template<>
	void SetUniform<unsigned int>(std::string uniformName, unsigned int uniformValue) {
		SetInt(uniformName, uniformValue, *ShaderID);
	};

	template<>
	void SetUniform<float>(std::string uniformName, float uniformValue)
	{
		SetFloat(uniformName, uniformValue, *ShaderID);
	};

	template<>
	void SetUniform<glm::vec2>(std::string uniformName, glm::vec2 uniformValue)
	{
		setVec2(uniformName, uniformValue, *ShaderID);
	};

	template<>
	void SetUniform<glm::vec3>(std::string uniformName, glm::vec3 uniformValue)
	{
		setVec3(uniformName, uniformValue, *ShaderID);
	};

	template<>
	void SetUniform<glm::vec4>(std::string uniformName, glm::vec4 uniformValue)
	{
		SetVec4(uniformName, uniformValue, *ShaderID);
	};

	template<>
	void SetUniform<glm::mat4>(std::string uniformName, glm::mat4 uniformValue)
	{
		SetMat4(uniformName, uniformValue, *ShaderID);
	};

	template<>
	void SetUniform<bool>(std::string uniformName, bool uniformValue)
	{
		SetBool(uniformName, uniformValue, *ShaderID);
	};

	Uniform(unsigned int *shaderID) : ShaderID{ shaderID } {};
	Uniform() {};
	~Uniform() {};

protected:

	// Uniform Updating
	static void SetBool(const std::string& name, bool value, unsigned int const& shaderID);
	static void SetInt(const std::string& name, int value, unsigned int const& shaderID);
	static void SetFloat(const std::string& name, float value, unsigned int const& shaderID);
	static void setVec2(const std::string& name, glm::vec2 value, unsigned int const& shaderID);
	static void setVec3(const std::string& name, glm::vec3 value, unsigned int const& shaderID);
	static void SetVec4(const std::string& name, glm::vec4 const& value, unsigned int const& shaderID);
	static void SetMat4(const std::string& name, glm::mat4 matrix, unsigned int const& shaderID);
	unsigned int* ShaderID;

};


class UniformArray : public Uniform
{
public:

	template<class T>
	void AddUniform(std::string name, T uniformType) {
		CHROMA_ERROR("Not supported Uniform Type!");
	};

	template<>
	void AddUniform<int>(std::string uniformName, int uniformValue) {
		m_IntUniforms.insert(std::make_pair(uniformName, uniformValue));
	};

	template<>
	void AddUniform<float>(std::string uniformName, float uniformValue)
	{
		m_FloatUniforms.insert(std::make_pair(uniformName, uniformValue));
	};

	template<>
	void AddUniform<glm::vec2>(std::string uniformName, glm::vec2 uniformValue)
	{
		m_Vec2Uniforms.insert(std::make_pair(uniformName, uniformValue));
	};

	template<>
	void AddUniform<glm::vec3>(std::string uniformName, glm::vec3 uniformValue)
	{
		m_Vec3Uniforms.insert(std::make_pair(uniformName, uniformValue));
	};

	template<>
	void AddUniform<glm::vec4>(std::string uniformName, glm::vec4 uniformValue)
	{
		m_Vec4Uniforms.insert(std::make_pair(uniformName, uniformValue));
	};

	template<>
	void AddUniform<glm::mat4>(std::string uniformName, glm::mat4 uniformValue)
	{
		m_Mat4Uniforms.insert(std::make_pair(uniformName, uniformValue));
	};

	template<>
	void AddUniform<unsigned int>(std::string uniformName, unsigned int uniformValue) {
		m_UIntUniforms.insert(std::make_pair(uniformName, uniformValue));
	};

	void SetUniforms(unsigned int shaderID);

	int size();

	UniformArray() {};
	~UniformArray() {};

	std::map<std::string, int>           m_IntUniforms;
	std::map<std::string, unsigned int>  m_UIntUniforms;
	std::map<std::string, float>         m_FloatUniforms;
	std::map<std::string, glm::vec2>     m_Vec2Uniforms;
	std::map<std::string, glm::vec3>     m_Vec3Uniforms;
	std::map<std::string, glm::vec4>     m_Vec4Uniforms;
	std::map<std::string, glm::mat4>     m_Mat4Uniforms;

};



#endif