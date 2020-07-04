#include "Material.h"
#include <render/Render.h>
#include <buffer/GBuffer.h>

namespace Chroma
{
	void Material::Destroy()
	{
		//// Textures
		//for (Texture& texture : m_TextureSet)
		//{
		//	texture.Destroy();
		//}
		//m_TextureSet.clear();
		//// Shader
		//m_Shader.Destroy();
	}

	void Material::Serialize(ISerializer* serializer)
	{
		// Texture Editor Property
		EditorProperty editorPrpty(Chroma::Type::EditorProperty::kMaterialTextureProperty);

		// Textures
		for (Texture& texture : m_TextureSet)
		{
			switch (texture.GetType())
			{
			case(Chroma::Type::Texture::kAlbedo):
			{
				serializer->AddProperty("kAlbedo", texture.GetSourcePath(), editorPrpty);
				break;
			}
			case(Chroma::Type::Texture::kNormal):
			{
				serializer->AddProperty("kNormal", texture.GetSourcePath(), editorPrpty);
				break;
			}
			case(Chroma::Type::Texture::kMetRoughAO):
			{
				serializer->AddProperty("kMetRoughAO", texture.GetSourcePath(), editorPrpty);
				break;
			}
			case(Chroma::Type::Texture::kTranslucency):
			{
				serializer->AddProperty("kTranslucency", texture.GetSourcePath(), editorPrpty);
				break;
			}
			default:
			{
				CHROMA_ERROR("Texture not supported for Serialization : {0}", "NONE");
				break;
			}
			}
		}

		// Material Uniform Property
		editorPrpty.m_Type = Chroma::Type::EditorProperty::kMaterialUniformProperty;
		// Uniforms
		// float
		for (auto& floatUniform : m_Uniforms.m_FloatUniforms)
			serializer->AddProperty(floatUniform.first.c_str(), &floatUniform.second, editorPrpty);
		// int
		for (auto& intUniform : m_Uniforms.m_IntUniforms)
			serializer->AddProperty(intUniform.first.c_str(), &intUniform.second, editorPrpty);
		// uInt
		for (auto& uIntUniform : m_Uniforms.m_UIntUniforms)
			serializer->AddProperty(uIntUniform.first.c_str(), &uIntUniform.second, editorPrpty);
		// vec2
		for (auto& vec2Uniform : m_Uniforms.m_Vec2Uniforms)
			serializer->AddProperty(vec2Uniform.first.c_str(), &vec2Uniform.second, editorPrpty);

		editorPrpty.m_Type = Chroma::Type::EditorProperty::kMaterialUniformColorProperty;
		// vec3
		for (auto& vec3Uniform : m_Uniforms.m_Vec3Uniforms)
			serializer->AddProperty(vec3Uniform.first.c_str(), &vec3Uniform.second, editorPrpty);
		// vec4
		for (auto& vec4Uniform : m_Uniforms.m_Vec4Uniforms)
			serializer->AddProperty(vec4Uniform.first.c_str(), &vec4Uniform.second, editorPrpty);
	}

	void Material::SetTextureSet(std::vector<Texture> newTextureSet)
	{
		if (newTextureSet.size() > 0)
		{
			for (unsigned int i = 0; m_TextureSet.size(); i++)
			{
				//bool skip{ false };
				//for (unsigned int j = 0; j < m_TextureSet.size(); j++)
				//{
				//	if (std::strcmp(m_TextureSet[j].GetSourcePath().data(), newTextureSet[j].GetSourcePath().data()) == 0)
				//	{
				//		skip = true;
				//		break;
				//	}
				//}
				//if (!skip)
				//	m_TextureSet.push_back(m_TextureSet[i]);
				m_TextureSet.push_back(m_TextureSet[i]);
			}
		}
	}

	void Material::AddTexture(Texture& newTexture)
	{
		//bool skip{ false };
		//for (unsigned int i = 0; i < m_TextureSet.size(); i++)
		//{
		//	skip = false;
		//	if (std::strcmp(m_TextureSet[i].GetSourcePath().data(), newTexture.GetSourcePath().data()) == 0)
		//	{
		//		skip = true;
		//		break;
		//	}
		//}
		//if (!skip)
		//{
		//	m_TextureSet.push_back(newTexture);
		//}
		m_TextureSet.push_back(newTexture);

	}

	Material::Material()
	{
		m_Shader = Shader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
		m_Uniforms.AddUniform("roughness", m_Roughness);
		m_Uniforms.AddUniform("color", m_Color);
		m_Uniforms.AddUniform("metalness", m_Metalness);
	}

	Material::~Material()
	{
	}

	void Material::UpdateUniforms(Shader& shader, Camera& RenderCam, const glm::mat4& modelTransform)
	{
		SetTransformUniforms(shader, RenderCam, modelTransform);
		SetMaterialUniforms(shader);
		SetTextureUniforms(shader);
	}

	void Material::SetTransformUniforms(Shader& shader, Camera& renderCam, const glm::mat4& modelTransfrom)
	{
		shader.SetUniform("model", modelTransfrom);
	}

	void Material::SetMaterialUniforms(Shader& shader)
	{
		shader.SetUniform("UseAlbedoMap", false);
		shader.SetUniform("UseNormalMap", false);
		shader.SetUniform("UseMetRoughAOMap", false);

		m_Uniforms.SetUniforms(shader.ShaderID);

		if (m_UsesGameTime)
		{
			shader.SetUniform("gameTime", (float)GAMETIME);
		}
	}

	void Material::SetTextureUniforms(Shader& shader)
	{
		// UV Modifiers
		shader.SetUniform("UVMultiply", GetUVMultiply());

		// updating shader's texture uniforms
		unsigned int diffuseNr{ 1 };
		unsigned int normalNr{ 1 };
		unsigned int roughnessNr{ 1 };
		unsigned int metalnessNr{ 1 };
		unsigned int metroughaoNr{ 1 };
		unsigned int aoNr{ 1 };
		unsigned int translucencyNr{ 1 };
		for (int i = 0; i < m_TextureSet.size(); i++)
		{
			// building the uniform name
			std::string name;
			std::string texturenum;
			Chroma::Type::Texture textureType = GetTextureSet()[i].GetType();

			switch (textureType)
			{
			case Chroma::Type::Texture::kAlbedo:
			{
				name = "material.texture_albedo";
				texturenum = std::to_string(diffuseNr++);
				// set use texture albedo
				shader.SetUniform("UseAlbedoMap", true);
				break;
			}
			case Chroma::Type::Texture::kNormal:
			{
				name = "material.texture_normal";
				texturenum = std::to_string(normalNr++);
				// set use texture normals
				shader.SetUniform("UseNormalMap", true);
				break;
			}
			case Chroma::Type::Texture::kMetRoughAO:
			{
				name = "material.texture_MetRoughAO";
				texturenum = std::to_string(metroughaoNr++);
				// set use texture metroughao
				shader.SetUniform("UseMetRoughAOMap", true);
				break;
			}
			case Chroma::Type::Texture::kMetalness:
			{
				name = "material.texture_metalness";
				texturenum = std::to_string(metalnessNr++);
				break;
			}
			case Chroma::Type::Texture::kRoughness:
			{
				name = "material.texture_roughness";
				texturenum = std::to_string(roughnessNr++);
				break;
			}
			case Chroma::Type::Texture::kAO:
			{
				name = "material.texture_ao";
				texturenum = std::to_string(aoNr++);
				break;
			}
			case Chroma::Type::Texture::kTranslucency:
			{
				name = "material.texture_translucency";
				texturenum = std::to_string(translucencyNr++);
				shader.SetUniform("UseTranslucencyMap", true);
				break;
			}
			}

			// Activate Texture before binding
			glActiveTexture(GL_TEXTURE0 + i);
			// Bind Texture
			glBindTexture(GL_TEXTURE_2D, GetTextureSet()[i].GetID());
			// Set Unitform
			shader.SetUniform((name + texturenum).c_str(), i);
		}

		if (m_IsForwardLit)
		{
			// Set LightSpace Matrix
			shader.SetUniform("lightSpaceMatrix", static_cast<ShadowBuffer*>(Chroma::Render::GetShadowBuffer())->GetLightSpaceMatrix());
			// Set PBR Lighting Texture Uniforms
			SetPBRLightingTextureUniforms(shader);
			// Shadows
			glActiveTexture(GL_TEXTURE0 + GetNumTextures() + 4);
			shader.SetUniform("shadowmap", GetNumTextures() + 4);
			glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<ShadowBuffer*>(Chroma::Render::GetShadowBuffer())->GetTexture());
		}
		if (m_UsesSceneNoise)
		{
			// BRDF LUT
			glActiveTexture(GL_TEXTURE0 + GetNumTextures() + 4);
			shader.SetUniform("noise", GetNumTextures() + 4);
			glBindTexture(GL_TEXTURE_2D, Chroma::Scene::GetSceneNoiseTex().GetID());
		}

		glActiveTexture(GL_TEXTURE0);
	}

	void Material::SetPBRLightingTextureUniforms(Shader& shader)
	{
		// Irradiance
		glActiveTexture(GL_TEXTURE0 + GetNumTextures() + 1);
		shader.SetUniform("irradianceMap", GetNumTextures() + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Chroma::Scene::GetIBL()->GetIrradianceMapID());
		// Prefilter Map
		glActiveTexture(GL_TEXTURE0 + GetNumTextures() + 2);
		shader.SetUniform("prefilterMap", GetNumTextures() + 2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Chroma::Scene::GetIBL()->GetPrefilterMapID());
		// BRDF LUT
		glActiveTexture(GL_TEXTURE0 + GetNumTextures() + 3);
		shader.SetUniform("brdfLUT", GetNumTextures() + 3);
		glBindTexture(GL_TEXTURE_2D, Chroma::Scene::GetIBL()->GetBRDFLUTID());
	}
}

