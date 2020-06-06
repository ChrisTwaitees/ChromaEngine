#include "BoxPrimitive.h"


void BoxPrimitive::setupBox()
{
	int stride = 8;
	GLsizei verts_size = boxData.size() * sizeof(boxData[0]);
	// VAO
	glGenVertexArrays(1, &m_MeshData.VAO);
	glBindVertexArray(m_MeshData.VAO);
	// VBO
	glGenBuffers(1, &m_MeshData.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_MeshData.VBO);
	glBufferData(GL_ARRAY_BUFFER, verts_size, &boxData[0], GL_STATIC_DRAW);

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

void BoxPrimitive::BindDrawVAO()
{
	glBindVertexArray(m_MeshData.VAO);
	glBindVertexArray(m_MeshData.VAO);
	glDrawArrays(GL_TRIANGLES, 0, boxData.size() / 8);
	glBindVertexArray(0); // reset to default
}

std::vector<ChromaVertex> BoxPrimitive::GetVertices()
{
	m_vertices.clear();
	for (int i = 0; i < boxData.size(); i+=8)
	{
		ChromaVertex newVert;
		glm::vec3 m_Position;
		glm::vec3 normal;
		glm::vec2 texcoords;
		int vertDataIndex = i;

		// positions
		for (int j = 0; j < 3; j++)
		{
			m_Position[j] = boxData[vertDataIndex];
			vertDataIndex++;
		}
		newVert.m_position = m_Position;
		vertDataIndex++;
		// normals
		for (int j = 0; j < 3; j++)
		{
			normal[j] = boxData[vertDataIndex];
			vertDataIndex++;
		}
		newVert.m_normal = normal;
		vertDataIndex++;
		// texcoords
		for (int j = 0; j < 2; j++)
		{
			texcoords[j] = boxData[vertDataIndex];
			vertDataIndex++;
		}
		newVert.m_texCoords = (texcoords);

		m_vertices.push_back(newVert);
	}
	return m_vertices;
}


BoxPrimitive::~BoxPrimitive()
{
}
