#include "Terrain.h"

void Terrain::initialize()
{
	// binding texture
	bindTexture(Texture(defaultTextureSource));
	// assigning shader
	pShader = new Shader(fragShaderSource, vtxShaderSource);

	// Setting Default Transform
	TerrainIdentityMatrix = glm::translate(TerrainIdentityMatrix, glm::vec3(0.0f, 15.0f, 0.0f));
	TerrainIdentityMatrix = glm::scale(TerrainIdentityMatrix, glm::vec3(30.0f));
	TerrainIdentityMatrix = glm::rotate(TerrainIdentityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	TerrainTransformMatrix = TerrainIdentityMatrix;
}

void Terrain::setupQuad()
{
	int stride = 8;
	GLsizei verts_size = quadData.size() * sizeof(quadData[0]);
	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts_size, &quadData[0], GL_STATIC_DRAW);

	// Vertex Shader Attribs
	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)0);
	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(sizeof(float) * 3));
	// uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(sizeof(float) * 6));

	glBindVertexArray(0);
}


void Terrain::BindDrawVAO()
{
	// draw mesh
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, quadData.size() / 8);
	glBindVertexArray(0);
}


void Terrain::setTerrainHeight(double newTerrainHeight)
{
	TerrainTransformMatrix = glm::translate(TerrainIdentityMatrix, glm::vec3(0.0f, newTerrainHeight, 0.0f));
}

glm::mat4 Terrain::getTransformationMatrix()
{
	return TerrainTransformMatrix;
}

Terrain::Terrain()
{
	initialize();
	setupQuad();
}


Terrain::~Terrain()
{
}
