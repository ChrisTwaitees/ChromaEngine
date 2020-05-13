#include "MeshComponent.h"
#include <entity/IEntity.h>
#include <scene/Scene.h>

MeshComponent::MeshComponent()
{
	Init();
}


MeshComponent::~MeshComponent()
{
	Destroy();
}

void MeshComponent::RebuildTransform()
{
	m_Transform = Chroma::Math::BuildMat4(m_Translation, m_Rotation, m_Scale);
}

void MeshComponent::SerializeMaterial(ISerializer*& serializer)
{
	// Texture Editor Property
	EditorProperty editorPrpty(Chroma::Type::EditorProperty::kMaterialTextureProperty);

	// Textures
	for (Texture& texture : GetTextureSet())
	{
		switch (texture.m_Type)
		{
		case(Texture::ALBEDO):
		{
			serializer->AddProperty("Texture_ALBEDO", &texture.GetSourcePath(), editorPrpty);
			break;
		}
		case(Texture::NORMAL):
		{
			serializer->AddProperty("Texture_NORMAL", &texture.GetSourcePath(), editorPrpty);
			break;
		}
		case(Texture::METROUGHAO):
		{
			serializer->AddProperty("Texture_METROUGHAO", &texture.GetSourcePath(), editorPrpty);
			break;
		}
		case(Texture::TRANSLUCENCY):
		{
			serializer->AddProperty("Texture_TRANSLUCENCY", &texture.GetSourcePath(), editorPrpty);
			break;
		}
		default : 
		{
			break;
		}
		}
	}

	// Material Uniform Property
	editorPrpty.m_Type = Chroma::Type::EditorProperty::kMaterialUniformProperty;
	// Uniforms
	// float
	for (auto& floatUniform : m_Material.GetUniformArray().m_FloatUniforms)
		serializer->AddProperty(floatUniform.first.c_str(), &floatUniform.second, editorPrpty);
	// int
	for (auto& intUniform : m_Material.GetUniformArray().m_IntUniforms)
		serializer->AddProperty(intUniform.first.c_str(), &intUniform.second, editorPrpty);
	// uInt
	for (auto& uIntUniform : m_Material.GetUniformArray().m_UIntUniforms)
		serializer->AddProperty(uIntUniform.first.c_str(), &uIntUniform.second, editorPrpty);
	// vec2
	for (auto& vec2Uniform : m_Material.GetUniformArray().m_Vec2Uniforms)
		serializer->AddProperty(vec2Uniform.first.c_str(), &vec2Uniform.second, editorPrpty);

	editorPrpty.m_Type = Chroma::Type::EditorProperty::kMaterialUniformColorProperty;
	// vec3
	for (auto& vec3Uniform : m_Material.GetUniformArray().m_Vec3Uniforms)
		serializer->AddProperty(vec3Uniform.first.c_str(), &vec3Uniform.second, editorPrpty);
	// vec4
	for (auto& vec4Uniform : m_Material.GetUniformArray().m_Vec4Uniforms)
		serializer->AddProperty(vec4Uniform.first.c_str(), &vec4Uniform.second, editorPrpty);

}

void MeshComponent::ProcessRenderFlags()
{
	Chroma::Scene::ProcessMeshComponentRenderFlags(this);
}


void MeshComponent::Init()
{
	// Set Type
	m_Type = Chroma::Type::Component::kMeshComponent ;

	CMPNT_INITIALIZED
}

void MeshComponent::Update()
{
}

void MeshComponent::Destroy()
{
	CMPNT_DESTROYED
}

void MeshComponent::Serialize(ISerializer*& serializer)
{
	CMPNT_SERIALIZE_BEGIN
	// Properties
	// Transform
	serializer->AddProperty("m_Translation", &m_Translation);
	serializer->AddProperty("m_Rotation", &m_Rotation);
	serializer->AddProperty("m_Scale", &m_Scale);

	// File Properties
	serializer->AddProperty("m_SourcePath", &m_SourcePath);

	// Material 
	SerializeMaterial(serializer);

}

void MeshComponent::CleanUp()
{
	CHROMA_INFO("Mesh Component : {}, Cleaned Up.", m_UID.data);
}

void MeshComponent::SetTransform(glm::mat4 const& newTransformMat)
{
	m_Transform = newTransformMat;
	m_Scale = Chroma::Math::GetScale(newTransformMat);
	m_Translation = Chroma::Math::GetTranslation(newTransformMat);
	m_Rotation = Chroma::Math::GetQuatRotation(newTransformMat);
}

glm::vec3 MeshComponent::GetWSTranslation()
{
	return GetParentEntity()->GetTranslation() + m_Translation;
}

glm::mat4 MeshComponent::GetWorldTransform()
{
	return GetParentEntity()->GetTransform() * GetTransform();
}

void MeshComponent::SetMaterial(const Material& newMaterial)
{
	// Set Material
	m_Material = newMaterial;

	// Refilter RenderPaths according to newMaterial Settings
	ProcessRenderFlags();
}
