#include "VertexBuffer.h"
#include <iostream>
#include <vector>

VertexBuffer::VertexBuffer(const std::vector<float> &verts)
{
	// set verts_size 
	num_verts = verts.size() / sizeof(verts[0]);
	// create VAO and VBO
	GLuint VBO;

	// VAO
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	GLsizei verts_size = verts.size() * sizeof(verts[0]);
	glBufferData(GL_ARRAY_BUFFER, verts_size, &verts[0], GL_STATIC_DRAW);

	// Vertex Shader Attribs
	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(1);
	// uvs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

}

void VertexBuffer::bind()
{
	glBindVertexArray(ID);
}

VertexBuffer::~VertexBuffer()
{

}