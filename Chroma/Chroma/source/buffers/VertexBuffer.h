#ifndef _VERTEXBUFFER_H_
#define _VERTEXBUFFER_H_
#include <glad/glad.h>
#include <vector>


class VertexBuffer
{
public:
	GLuint id;
	GLuint num_verts;
	VertexBuffer(const std::vector<float> &verts);
	VertexBuffer();
	~VertexBuffer();
	void bind();
};
#endif

