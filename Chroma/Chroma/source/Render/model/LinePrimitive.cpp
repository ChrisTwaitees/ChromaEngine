#include "LinePrimitive.h"


void LinePrimitive::setupMesh()
{
	int stride = 8;
	GLsizei verts_size = lineData.size() * sizeof(lineData[0]);
	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts_size, &lineData[0], GL_STATIC_DRAW);

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

void LinePrimitive::BindDrawVAO()
{
	m_shader->use();
	m_shader->setVec3("start", start);
	m_shader->setVec3("end", end);
	m_shader->setFloat("thickness", thickness);

	glDisable(GL_CULL_FACE);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, lineData.size() / 8);
	glBindVertexArray(0); // reset to default
	glEnable(GL_CULL_FACE);
}

LinePrimitive::LinePrimitive(glm::vec3 start_val, glm::vec3 end_val, float thicknessVal)
{
	m_shader = new Shader(fragSrc, vtxSrc);
	start = start_val;
	end = end_val;
	thickness = thicknessVal;
	setupMesh();
}

LinePrimitive::~LinePrimitive()
{
}
