#ifndef _CHROMA_UNIFORM_
#define _CHROMA_UNIFORM_

#include <map>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


template<typename UniformType>
class UniformArray
{
public:
	std::map<std::string, UniformType> uniforms;

	void addUniform(std::string uniformName, UniformType uniformValue) {
		uniforms.insert(std::make_pair(uniformName, uniformValue));
	};

	int size() { return uniforms.size(); };

	UniformArray() {};
	~UniformArray() {};
};


class Uniform
{
private:
	UniformArray<int> intUniforms;
	UniformArray<float> floatUniforms;
	UniformArray<glm::vec2> vec2Uniforms;
	UniformArray<glm::vec3> vec3Uniforms;
	UniformArray<glm::mat4> mat4Uniforms;

	unsigned int* ShaderID;

public:
	// Adding new Uniforms

	template<class T>
	void addUniform(std::string name, T uniformType) {
		std::cout << "not a supported uniform type!" << std::endl;
	};

	template<>
	void addUniform<int>(std::string uniformName, int uniformType) {
		intUniforms.addUniform(uniformName, uniformType);
	};

	template<>
	void addUniform<float>(std::string uniformName, float uniformVlaue)
	{
		floatUniforms.addUniform(uniformName, uniformVlaue);
	};

	template<>
	void addUniform<glm::vec2>(std::string uniformName, glm::vec2 uniformVlaue)
	{
		vec2Uniforms.addUniform(uniformName, uniformVlaue);
	};

	template<>
	void addUniform<glm::vec3>(std::string uniformName, glm::vec3 uniformVlaue)
	{
		vec3Uniforms.addUniform(uniformName, uniformVlaue);
	};

	template<>
	void addUniform<glm::mat4>(std::string uniformName, glm::mat4 uniformVlaue)
	{
		mat4Uniforms.addUniform(uniformName, uniformVlaue);
	};

	// Renders
	void updateUniforms();

	// Uniform Updating
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setMat4(const std::string& name, glm::mat4 matrix) const;


	Uniform(unsigned int *shaderID) : ShaderID{ shaderID } {};
	~Uniform() {};

};


#endif