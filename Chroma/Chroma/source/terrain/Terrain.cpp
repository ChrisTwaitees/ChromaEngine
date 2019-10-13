#include "Terrain.h"

void Terrain::initialize()
{
	// binding texture
	bindTexture(Texture(defaultTextureSource));
	// assigning shader
	shader = Shader(fragShaderSource, vtxShaderSource);

	// updating transform matrix;
	TerrainTransformMatrix = glm::mat4(1);
	TerrainTransformMatrix = glm::translate(TerrainTransformMatrix, glm::vec3(0.0f, 15.0f, 0.0f));
	TerrainTransformMatrix = glm::scale(TerrainTransformMatrix, glm::vec3(30.0f));
	TerrainTransformMatrix = glm::rotate(TerrainTransformMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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

void Terrain::updateMaterialUniforms(Shader& shader)
{
	shader.setFloat("material.ambientBrightness", 0.06f);
	shader.setFloat("material.roughness", 64.0f);
	shader.setFloat("material.specularIntensity", 1.0f);
	shader.setFloat("material.cubemapIntensity", 1.0f);
}

void Terrain::BindDrawVAO()
{
	// draw mesh
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, quadData.size() / 8);
	glBindVertexArray(0);
}

void Terrain::Draw(Camera& RenderCamera, std::vector<Light*>& Lights, glm::mat4& transformMatrix)
{
	pShader->use();
	updateUniforms(*pShader, Lights, RenderCamera, TerrainTransformMatrix);
	BindDrawVAO();
}

void Terrain::Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*>& Lights, glm::mat4& transformMatrix)
{
	shader.use();
	updateUniforms(shader, Lights, RenderCamera, TerrainTransformMatrix);
	BindDrawVAO();
}

Terrain::Terrain()
{
	initialize();
	setupQuad();
}


Terrain::~Terrain()
{
}
