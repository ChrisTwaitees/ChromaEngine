#include "PlanePrimitive.h"

PlanePrimitive::PlanePrimitive()
{
	setupQuad();
}

PlanePrimitive::~PlanePrimitive()
{

}

void PlanePrimitive::setupQuad()
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

void PlanePrimitive::Draw(Shader& shader)
{
	shader.use();
	updateTransformUniforms(shader, *pCamera, modelMat);
	updateTextureUniforms(shader);
	// draw mesh
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, quadData.size() / 8);
	glBindVertexArray(0); // reset to default
}
