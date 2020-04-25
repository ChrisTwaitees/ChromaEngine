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
	inline void SetShader(Shader& newShader) {  m_Shader = newShader; }
	inline void SetShader(const char* shaderFRAGSourcePath, const char* shaderVTXSourcePath, const char* shaderGEOMSourcePath = "") 
	{
		m_Shader = Shader(shaderFRAGSourcePath, shaderVTXSourcePath, shaderGEOMSourcePath);
	}

	// Texture
	std::vector<Texture>& GetTextureSet() { return m_TextureSet; };
	void SetTextureSet(std::vector<Texture> newTextureSet);
	void AddTexture(Texture& newTexture);

	// Default Properties
	inline glm::vec2 GetUVMultiply() { return m_UVMultiply; }
	inline void SetUVMultiply(const glm::vec2& newUV) { m_UVMultiply = newUV; }
	inline void SetUVMultiply(const float& newUVFactor) { m_UVMultiply = glm::vec2(newUVFactor); };

	inline glm::vec4 GetColor() { return m_Color; }
	inline void SetColor(const glm::vec4& newColor) { m_Color = newColor; }

	inline float GetMetalness() { return m_Metalness; }
	inline void SetMetalness(const float& newMetalness) { m_Metalness = newMetalness; }

	inline float GetRoughness() { return m_Roughness; }
	inline void SetRoughness(const float& newRoughness) { m_Roughness = newRoughness; }

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

	// Render Flags
	inline void SetIsRenderable(bool const& check) { m_IsRenderable = check; };
	inline bool GetIsRenderable() { return m_IsRenderable; }
	inline void SetIsTransparent(bool const& check) { m_IsTransparent = check; };
	inline bool GetIsTransparent() { return m_IsTransparent; }
	inline void SetIsLit(bool const& check) { m_IsLit = check; };
	inline bool GetIsLit() { return m_IsLit; }
	inline void SetIsUnlit(bool const& check) { m_IsUnlit = check; };
	inline bool GetIsUnlit() { return m_IsUnlit; }
	inline void SetIsForwardLit(bool const& check) { m_IsForwardLit = check; };
	inline bool GetIsForwardLit() { return m_IsForwardLit; }
	inline void SetIsDoubleSided(bool const& check) { m_IsDoubleSided = check; };
	inline bool GetIsDoubleSided() { return m_IsDoubleSided; }
	inline void SetCastsShadows(bool const& check) { m_CastShadows = check; };
	inline bool GetCastsShadows() { return m_CastShadows; }
	inline void SetReceivesShadows(bool const& check) { m_ReceivesShadows = check; };
	inline bool GetReceivesShadows() { return m_ReceivesShadows; }


	Material();
	~Material();

private:
	// Shader
	Shader m_Shader;

	// TextureSet
	std::vector<Texture> m_TextureSet;

	// Uniforms
	UniformArray m_Uniforms;

	// Defaults
	glm::vec2 m_UVMultiply{ 1.0 };
	float m_Roughness{ 0.5f };
	glm::vec4 m_Color{ 1.0, 0.0, 0.0, 0.5 };
	float m_Metalness{ 0.0f };
	float m_Metalness{ 0.0f };




	// Render Flags
	bool m_IsRenderable{ true };
	bool m_IsLit{ true };
	bool m_IsTransparent{ false };
	bool m_IsUnlit{ false };
	bool m_IsForwardLit{ false };
	bool m_CastShadows{ true };
	bool m_ReceivesShadows{ true };
	bool m_IsDoubleSided{ false };
};

#endif

