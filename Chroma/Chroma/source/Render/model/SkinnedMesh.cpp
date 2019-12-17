#include "SkinnedMesh.h"



void SkinnedMesh::CalculateBBox()
{
	glm::vec3 newMinBBox(99999.00, 99999.00, 99999.00);
	glm::vec3 newMaxBBox(0.0, 0.0, 0.0);
	for (ChromaSkinnedVertex& vert : m_skinnedVertices)
	{
		newMinBBox = glm::min(newMinBBox, vert.GetPosition());
		newMaxBBox = glm::max(newMaxBBox, vert.GetPosition());
	}
	// re-establishing min and max bboxes
	m_BBoxMin = newMinBBox;
	m_BBoxMax = newMaxBBox;
}

void SkinnedMesh::setupMesh()
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
	glBufferData(GL_ARRAY_BUFFER, m_skinnedVertices.size() * sizeof(ChromaSkinnedVertex), &m_skinnedVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
		&m_indices[0], GL_STATIC_DRAW);

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
}

std::pair<glm::vec3, glm::vec3> SkinnedMesh::GetBBox()
{
	return std::make_pair(m_BBoxMin, m_BBoxMax);
}


void SkinnedMesh::SetJointUniforms(Shader& skinnedShader)
{
	// set transform per joint
	for (auto const& IDJoint : m_Skeleton.GetIndexedNamedJoints())
	{
		std::string jntUniformName = "aJoints[" + std::to_string(IDJoint.first.first) + "]";
		skinnedShader.setUniform(jntUniformName, IDJoint.second.GetModelInverseBindTransform());
	}
}

SkinnedMesh::SkinnedMesh(std::vector<ChromaSkinnedVertex>& vertices_val, std::vector<unsigned int>& indices_val, std::vector<Texture>& textures_val, Skeleton& skeleton_val, glm::mat4 rootTransform_val)
{
	m_IsRenderable = true;
	m_Skeleton = skeleton_val;
	m_Skeleton.CalculateJointBindTransforms();
	m_skinnedVertices = vertices_val;
	m_indices = indices_val;
	m_textures = textures_val;
	m_RootTransform = rootTransform_val;
	m_RootTransformInversed = glm::inverse(rootTransform_val);

	setupMesh();
}


SkinnedMesh::~SkinnedMesh()
{
	//delete m_Skeleton;
}
