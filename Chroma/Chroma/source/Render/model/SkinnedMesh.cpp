#include "SkinnedMesh.h"
#include <entity/IEntity.h>
#include <resources/ModelLoader.h>




void SkinnedMesh::SetupMesh()
{
	// Generate buffers
	// Vertex Array Object Buffer
	glGenVertexArrays(1, &VAO);
	// Vertex Buffer and Element Buffer
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_SkinnedVertices.size() * sizeof(ChromaSkinnedVertex), &m_SkinnedVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int),
		&m_Indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaSkinnedVertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaSkinnedVertex), (void*)offsetof(ChromaSkinnedVertex, ChromaSkinnedVertex::m_normal));
	// vertex uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ChromaSkinnedVertex), (void*)offsetof(ChromaSkinnedVertex, ChromaSkinnedVertex::m_texCoords));
	// vertex tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaSkinnedVertex), (void*)offsetof(ChromaSkinnedVertex, ChromaSkinnedVertex::m_tangent));
	// vertex bitangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaSkinnedVertex), (void*)offsetof(ChromaSkinnedVertex, ChromaSkinnedVertex::m_bitangent));
	// vertex bone IDs
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, MAX_VERT_INFLUENCES, GL_INT, sizeof(ChromaSkinnedVertex), (void*)offsetof(ChromaSkinnedVertex, ChromaSkinnedVertex::m_jointIDs));
	// vertex bone weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, MAX_VERT_INFLUENCES, GL_FLOAT, GL_FALSE, sizeof(ChromaSkinnedVertex), (void*)offsetof(ChromaSkinnedVertex, ChromaSkinnedVertex::m_jointWeights));

	glBindVertexArray(0);

	// BBOX
	CalculateBBox();
	CalculateCentroid();

}

glm::mat4 SkinnedMesh::GetWorldTransform()
{
	return GetParentEntity()->GetTransform();
}

std::vector<ChromaVertex> SkinnedMesh::GetVertices()
{

	std::vector<ChromaVertex> verts;
	for (ChromaSkinnedVertex const& vert : m_SkinnedVertices)
	{
		verts.push_back(static_cast<ChromaVertex>(vert));
	}
	return verts;

}

void SkinnedMesh::SetJointUniforms(Shader& skinnedShader)
{
	m_Skeleton.SetJointUniforms(skinnedShader);
}

void SkinnedMesh::Init()
{
	m_Type = Chroma::Type::Component::kSkinnedMeshComponent;
	CMPNT_INITIALIZED
}

void SkinnedMesh::Destroy()
{
	// Material
	m_Material.Destroy();
	// verts
	m_SkinnedVertices.clear();
	// Skeleton
	m_Skeleton.Destroy();
	// vao
	glDeleteVertexArrays(1, &VAO);
	// buffers
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	CMPNT_DESTROYED
}

void SkinnedMesh::Serialize(ISerializer*& serializer)
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


SkinnedMesh::SkinnedMesh(std::vector<ChromaSkinnedVertex>& vertices_val, std::vector<unsigned int>& indices_val, std::vector<Texture>& textures_val, Skeleton& skeleton_val, glm::mat4 rootTransform_val)
{
	// Renderables
	m_IsSkinned = true;
	// Skeleton
	m_Skeleton = skeleton_val;
	m_Skeleton.SetParentComponentUID(m_UID);
	// Verts
	m_SkinnedVertices = vertices_val;
	m_Indices = indices_val;
	// Transforms
	m_RootTransform = rootTransform_val;
	m_RootTransformInversed = glm::inverse(rootTransform_val);
	// Textures
	m_Material.SetTextureSet(textures_val);
	// Build Mesh
	SetupMesh();
}

SkinnedMesh::SkinnedMesh(MeshData const& newMeshData)
{
	// Renderables
	m_IsSkinned = true;
	// Skeleton
	m_Skeleton = newMeshData.skeleton;
	m_Skeleton.SetParentComponentUID(m_UID);
	// Verts
	m_SkinnedVertices = newMeshData.skinnedVerts;
	m_Indices = newMeshData.indices;
	// Textures
	m_Material.SetTextureSet(newMeshData.textures);
	// Build Mesh
	SetupMesh();
}

SkinnedMesh::SkinnedMesh(std::string const& sourcePath)
{
	MeshData newMeshData = Chroma::ModelLoader::Load(sourcePath)[0];
	// Renderables
	m_IsSkinned = true;
	// Skeleton
	m_Skeleton = newMeshData.skeleton;
	m_Skeleton.SetParentComponentUID(m_UID);
	// Verts
	m_SkinnedVertices = newMeshData.skinnedVerts;
	m_Indices = newMeshData.indices;
	// Textures
	m_Material.SetTextureSet(newMeshData.textures);
	// Build Mesh
	SetupMesh();

}


SkinnedMesh::~SkinnedMesh()
{
}
