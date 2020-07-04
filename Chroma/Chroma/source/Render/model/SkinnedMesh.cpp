#include "SkinnedMesh.h"
#include <entity/IEntity.h>
#include <resources/ModelLoader.h>


namespace Chroma
{

	void SkinnedMesh::SetupMesh()
	{
		m_MeshData.isRenderBuffersInitialized = false;

		// Generate buffers
		// Vertex Array Object Buffer
		glGenVertexArrays(1, &m_MeshData.VAO);
		// Vertex Buffer and Element Buffer
		glGenBuffers(1, &m_MeshData.VBO);
		glGenBuffers(1, &m_MeshData.EBO);

		// Bind buffers
		glBindVertexArray(m_MeshData.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_MeshData.VBO);
		glBufferData(GL_ARRAY_BUFFER, m_MeshData.skinnedVerts.size() * sizeof(ChromaSkinnedVertex), &m_MeshData.skinnedVerts[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshData.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_MeshData.indices.size() * sizeof(unsigned int),
			&m_MeshData.indices[0], GL_STATIC_DRAW);

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
		// vertex colors 
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(ChromaSkinnedVertex), (void*)offsetof(ChromaSkinnedVertex, ChromaSkinnedVertex::m_color));
		// second UV set
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(ChromaSkinnedVertex), (void*)offsetof(ChromaSkinnedVertex, ChromaSkinnedVertex::m_texCoords2));

		glBindVertexArray(0);


		// Skeleton
		m_Skeleton = m_MeshData.skeleton;
		m_Skeleton.SetParentComponentUID(m_UID);

		// Cleanup
		CleanUp();

		// Mark Meshdata initialized
		m_MeshData.isRenderBuffersInitialized = true;
	}

	glm::mat4 SkinnedMesh::GetWorldTransform()
	{
		return GetParentEntity()->GetTransform();
	}

	std::vector<ChromaVertex> SkinnedMesh::GetVertices()
	{
		std::vector<ChromaVertex> verts;
		for (ChromaSkinnedVertex const& vert : m_MeshData.skinnedVerts)
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
		m_MeshData.skinnedVerts.clear();
		// indices
		m_MeshData.indices.clear();
		// Skeleton
		m_Skeleton.Destroy();
		// vao
		glDeleteVertexArrays(1, &m_MeshData.VAO);
		// buffers
		glDeleteBuffers(1, &m_MeshData.VBO);
		glDeleteBuffers(1, &m_MeshData.EBO);

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
		serializer->AddProperty("m_SourcePath", &m_MeshData.sourcePath);

		// Material 
		m_Material.Serialize(serializer);
	}

	void SkinnedMesh::LoadFromFile(const std::string& sourcePath)
	{
		// Resource Manager Async
		Chroma::ResourceManager::LoadModel(sourcePath, &m_MeshData);

		// Initialize Mesh
		SetupMesh();
	}

	void SkinnedMesh::CleanUp()
	{
		// verts
		m_MeshData.verts.clear();
		m_MeshData.skinnedVerts.clear();
		// skeleton
		m_MeshData.skeleton.Destroy();
		// textures
		m_MeshData.textures.clear();

		CHROMA_INFO("Skinned Mesh Component : {0} Cleaned Up", m_UID.m_Data);
	}


	SkinnedMesh::SkinnedMesh(std::vector<ChromaSkinnedVertex>& vertices_val, std::vector<unsigned int>& indices_val, std::vector<Texture>& textures_val, Skeleton& skeleton_val, glm::mat4 rootTransform_val)
	{
		// Renderables
		m_MeshData.isSkinned = true;
		// Skeleton
		m_MeshData.skeleton = skeleton_val;
		m_Skeleton = skeleton_val;
		m_Skeleton.SetParentComponentUID(m_UID);
		// Verts
		m_MeshData.skinnedVerts = vertices_val;
		m_MeshData.indices = indices_val;
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
		m_MeshData = newMeshData;
	
		// Skeleton
		m_Skeleton = m_MeshData.skeleton;
		m_Skeleton.SetParentComponentUID(m_UID);

		// Textures
		m_Material.SetTextureSet(m_MeshData.textures);

		// Build Mesh
		SetupMesh();
	}

	SkinnedMesh::SkinnedMesh(std::string const& sourcePath)
	{
		LoadFromFile(sourcePath);
	}


	SkinnedMesh::~SkinnedMesh()
	{
	}
}
