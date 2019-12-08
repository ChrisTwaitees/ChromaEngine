#include "SkinnedMesh.h"



void SkinnedMesh::calcBBox()
{
}

void SkinnedMesh::calcCentroid()
{
}

void SkinnedMesh::setupMesh()
{
}

std::pair<glm::vec3, glm::vec3> SkinnedMesh::getBBox()
{
	return std::pair<glm::vec3, glm::vec3>();
}

void SkinnedMesh::BindDrawVAO()
{
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
