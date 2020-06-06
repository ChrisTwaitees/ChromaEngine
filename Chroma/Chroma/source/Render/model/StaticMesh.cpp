#include "StaticMesh.h"
#include <scene/Scene.h>
#include <resources/ModelLoader.h>


void StaticMesh::Init()
{
	m_Type = Chroma::Type::Component::kStaticMeshComponent;
	CMPNT_INITIALIZED
}


void StaticMesh::UpdateUniforms(Shader& shader, Camera& RenderCam)
{
	m_Material.UpdateUniforms(shader, RenderCam, GetWorldTransform());
}


void StaticMesh::UpdateTextureUniforms(Shader& shader)
{
	m_Material.UpdateTextureUniforms(shader);
}


void StaticMesh::UpdateTransformUniforms(Shader& shader, Camera& renderCam)
{
	m_Material.UpdateTransformUniforms(shader, renderCam, GetWorldTransform());
}


void StaticMesh::UpdateMaterialUniforms(Shader& shader)
{
	m_Material.UpdateMaterialUniforms(shader);
}


void StaticMesh::SetupMesh()
{
	// Generate buffers
	// Vertex Array Object Buffer
	glGenVertexArrays(1, &m_MeshData.VAO);
	// Vertex Buffer and Element Buffer
	glGenBuffers(1, &m_MeshData.VBO);
	glGenBuffers(1, &m_MeshData.EBO);

	// Bind buffers
	glBindVertexArray(m_MeshData.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_MeshData.VBO);
	glBufferData(GL_ARRAY_BUFFER, m_MeshData.verts.size() * sizeof(ChromaVertex), &m_MeshData.verts[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshData.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_MeshData.indices.size() * sizeof(unsigned int),
		&m_MeshData.indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)offsetof(ChromaVertex, ChromaVertex::m_normal));
	// vertex uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)offsetof(ChromaVertex, ChromaVertex::m_texCoords));
	// vertex tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)offsetof(ChromaVertex, ChromaVertex::m_tangent));
	// vertex bitangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)offsetof(ChromaVertex, ChromaVertex::m_bitangent));
	// vertex colors // is at the 7th index as 5 and 6 are used for skinning
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)offsetof(ChromaVertex, ChromaVertex::m_color));
	// second UV set
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)offsetof(ChromaVertex, ChromaVertex::m_texCoords2));

	glBindVertexArray(0);

	// Cleanup
	CleanUp();

	// Mesh complete
	m_MeshData.isRenderBuffersInitialized = true;
}


void StaticMesh::Draw(Shader& shader)
{
	//shader.Use();
	shader.Use();
	// draw mesh
	BindDrawVAO();
}

void StaticMesh::Draw(Camera& RenderCamera)
{
	GetShader().Use();
	UpdateUniforms(GetShader(), RenderCamera);
	BindDrawVAO();
}

void StaticMesh::Draw(Shader& shader, Camera& RenderCamera)
{
	shader.Use();
	UpdateUniforms(shader, RenderCamera);
	BindDrawVAO();
}

void StaticMesh::DrawUpdateMaterials(Shader& shader)
{
	shader.Use();
	UpdateMaterialUniforms(shader);
	UpdateTextureUniforms(shader);
	BindDrawVAO();
}

void StaticMesh::DrawUpdateTransforms(Camera& renderCam)
{
	m_Material.Use();
	UpdateTransformUniforms(GetShader(), renderCam);
	BindDrawVAO();
}

void StaticMesh::BindDrawVAO()
{
	if (m_MeshData.isRenderBuffersInitialized)
	{
		glBindVertexArray(m_MeshData.VAO);
		glDrawElements(GL_TRIANGLES, m_MeshData.indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); // reset to default
	}
	else if (m_MeshData.isRenderBuffersInitialized == false && m_MeshData.isLoaded)
		SetupMesh();
}

void StaticMesh::Destroy()
{
	// Material 
	m_Material.Destroy();
	// verts
	m_MeshData.verts.clear();
	// indices
	m_MeshData.indices.clear();
	// vao
	glDeleteVertexArrays(1, &m_MeshData.VAO);
	// buffers
	glDeleteBuffers(1, &m_MeshData.VBO);
	glDeleteBuffers(1, &m_MeshData.EBO);

	CMPNT_DESTROYED
}

void StaticMesh::Serialize(ISerializer*& serializer)
{
	CMPNT_SERIALIZE_BEGIN

	// Properties
	// Transform
	serializer->AddProperty("m_Translation", &m_Translation);
	serializer->AddProperty("m_Rotation", &m_Rotation);
	serializer->AddProperty("m_Scale", &m_Scale);

	// File Properties
	serializer->AddProperty("m_SourcePath", &m_MeshData.sourcePath);

	// Material 
	m_Material.Serialize(serializer);
}


void StaticMesh::CleanUp()
{
	// verts
	m_MeshData.verts.clear();
	// textures
	m_MeshData.textures.clear();
	CHROMA_INFO("Static Mesh Component : {0} Cleaned Up", m_UID.m_Data );
}


void StaticMesh::SetMat4(std::string name, glm::mat4 value)
{
	GetShader().Use();
	GetShader().SetUniform(name, value);
}

void StaticMesh::SetInt(std::string name, int value)
{
	GetShader().Use();
	GetShader().SetUniform(name, value);
}

void StaticMesh::SetFloat(std::string name, float value)
{
	GetShader().Use();
	GetShader().SetUniform(name, value);
}

StaticMesh::StaticMesh(std::vector<ChromaVertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val)
{
	m_MeshData.verts = vertices_val;
	m_MeshData.indices = indices_val;
	m_Material.SetTextureSet(textures_val);

	SetupMesh();
}

StaticMesh::StaticMesh(MeshData const& newMeshData)
{
	m_MeshData = newMeshData;
	m_Material.SetTextureSet(newMeshData.textures);

	SetupMesh();
}

void StaticMesh::LoadFromFile(const std::string& sourcePath)
{
	Chroma::ResourceManager::LoadModel(sourcePath, &m_MeshData);
}

static void testFunc(std::string arg)
{
	CHROMA_INFO("Test {0}", arg);
}

StaticMesh::StaticMesh(const std::string& sourcePath)
{
	LoadFromFile(sourcePath);

}

StaticMesh::StaticMesh()
{
}

StaticMesh::~StaticMesh()
{
	Destroy();
}

