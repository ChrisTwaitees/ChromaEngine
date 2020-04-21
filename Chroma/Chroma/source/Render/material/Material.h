#ifndef CHROMA_MATERIAL_H
#define CHROMA_MATERIAL_H

#include <common/PrecompiledHeader.h>
#include <shader/Shader.h>
#include <shader/Uniform.h>
#include <texture/Texture.h>

class Material
{
public:
	//Funcs
	inline void Use() { m_Shader.Use(); };
	void Destroy();

	// Shader
	Shader& GetShader() { return m_Shader; };
	inline void SetShader(const Shader& newShader) { m_Shader = newShader; }
	inline void SetShader(const char* shaderFRAGSourcePath, const char* shaderVTXSourcePath, const char* shaderGEOMSourcePath = "") 
	{
		m_Shader.Destroy();  m_Shader = Shader(shaderFRAGSourcePath, shaderVTXSourcePath, shaderGEOMSourcePath);
	}

	// Texture
	std::vector<Texture>& GetTextureSet() { return m_TextureSet; };
	void SetTextureSet(std::vector<Texture> newTextureSet);
	void AddTexture(Texture& newTexture);

	inline glm::vec2 GetUVMultiply() { return m_UVMultiply; }
	inline void SetUVMultiply(const glm::vec2& newUV) { m_UVMultiply = newUV; }
	inline void SetUVMultiply(const float& newUVFactor) { m_UVMultiply = glm::vec2(newUVFactor); };

	// Uniform
	UniformArray& GetUniformArray() { return m_Uniforms; };
	inline void SetUniforms() { m_Uniforms.SetUniforms(m_Shader.ShaderID); }

	template<typename UniformType>
	void AddUniform(std::string uniformName, UniformType uniformValue) {
		m_Uniforms.AddUniform(uniformName, uniformValue);
	};

	template<typename UniformType>
	void SetUniform(std::string uniformName, UniformType uniformValue) {
		m_Shader.SetUniform(uniformName, uniformValue);
	};

	Material() {};
	~Material() {};

private:
	Shader m_Shader{ "resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl" };
	UniformArray m_Uniforms;
	std::vector<Texture> m_TextureSet;
	glm::vec2 m_UVMultiply{ 1.0 };
};

#endif

