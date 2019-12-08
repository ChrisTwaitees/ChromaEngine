#include "SkinnedMesh.h"



void SkinnedMesh::calcBBox()
{
}

void SkinnedMesh::calcCentroid()
{
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

	glBindVertexArray(0);
}

std::pair<glm::vec3, glm::vec3> SkinnedMesh::getBBox()
{
	return std::pair<glm::vec3, glm::vec3>();
}

void SkinnedMesh::BindDrawVAO()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // reset to default
}

SkinnedMesh::SkinnedMesh(std::vector<ChromaSkinnedVertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val)
{
	isRenderable = true;
	m_skinnedVertices = vertices_val;
	m_indices = indices_val;
	m_textures = textures_val;

	setupMesh();
}


SkinnedMesh::~SkinnedMesh()
{
}
