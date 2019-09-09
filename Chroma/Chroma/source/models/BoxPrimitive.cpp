#include "BoxPrimitive.h"


void BoxPrimitive::setupBox()
{
	int stride = 8;
	GLsizei verts_size = boxData.size() * sizeof(boxData[0]);
	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

void BoxPrimitive::Draw(Shader shader)
{
	// updating shader's texture uniforms
	unsigned int diffuseNr{ 0 };
	unsigned int specularNr{ 0 };
	for (int i = 0; i < textures.size(); i++)
	{
		// building the uniform name
		std::string name;
		std::string texturenum;
		if (textures[i].type == Texture::DIFFUSE)
		{
			name = "diffuse";
			texturenum = std::to_string(i);
			diffuseNr++;
		}
		if (textures[i].type == Texture::SPECULAR)
		{
			name = "specular";
			texturenum = std::to_string(i);
			specularNr++;
		}
		// setting uniform and binding texture
		shader.setFloat("material." + name + texturenum, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	// activate texture
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, boxData.size()/8);
	glBindVertexArray(0);
}

BoxPrimitive::~BoxPrimitive()
{
}
