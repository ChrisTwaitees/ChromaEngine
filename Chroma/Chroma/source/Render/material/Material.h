#ifndef CHROMA_MATERIAL_H
#define CHROMA_MATERIAL_H

#include <common/PrecompiledHeader.h>
#include <shader/Shader.h>
#include <shader/Uniform.h>
#include <texture/Texture.h>

class Material
{
public:
	// Shader
	Shader& GetShader() { return m_Shader; };
	inline void SetShader(const Shader& newShader) { m_Shader = newShader; }
	inline void SetShader(const char* shaderFRAGSourcePath, const char* shaderVTXSourcePath, const char* shaderGEOMSourcePath = "") 
	{ m_Shader = Shader(shaderFRAGSourcePath, shaderVTXSourcePath, shaderGEOMSourcePath); }

	// Texture
	std::set<Texture>& GetTextureSet() { return m_TextureSet; };
	inline void SetTextureSet(const std::set<Texture>& newTextureSet) { m_TextureSet = newTextureSet; }
	inline void AddTexture(const Texture& newTexture) { m_TextureSet.emplace(newTexture); };

	// Uniform
	Uniform& GetUniformArray() { return m_Shader.GetUniformArray(); };
	inline void SetUniformArray(const Uniform& newUniform) { m_Shader.SetUniformArray(newUniform); }
	inline void SetUniforms() { m_Shader.SetUniforms(); }

	template<typename UniformType>
	void AddUniform(std::string uniformName, UniformType uniformValue) {
		m_Shader.AddUniform(uniformName, uniformValue);
	};

	template<typename UniformType>
	void SetUniform(std::string uniformName, UniformType uniformValue) {
		m_Shader.SetUniform(uniformName, uniformValue);
	};

	Material() {};
	~Material() {};

private:
	Shader m_Shader;
	std::set<Texture> m_TextureSet;
};

#endif

