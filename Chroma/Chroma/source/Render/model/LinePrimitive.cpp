#include "LinePrimitive.h"


void LinePrimitive::SetupMesh()
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
	GetShader().Use();
	GetShader().SetUniform("start", start);
	GetShader().SetUniform("end", end);
	GetShader().SetUniform("thickness", thickness);

	glDisable(GL_CULL_FACE);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, lineData.size() / 8);
	glBindVertexArray(0); // reset to default
	glEnable(GL_CULL_FACE);
}

LinePrimitive::LinePrimitive(glm::vec3 start_val, glm::vec3 end_val, float thicknessVal)
{
	m_Material.SetShader(fragSrc.c_str(), vtxSrc.c_str());
	start = start_val;
	end = end_val;
	thickness = thicknessVal;
	SetupMesh();
}

LinePrimitive::~LinePrimitive()
{
}
