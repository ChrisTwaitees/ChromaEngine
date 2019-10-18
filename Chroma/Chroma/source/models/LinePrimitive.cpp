#include "LinePrimitive.h"

void LinePrimitive::Draw(Shader& shader)
{
	shader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE, 0, 2);
}

LinePrimitive::~LinePrimitive()
{
}
