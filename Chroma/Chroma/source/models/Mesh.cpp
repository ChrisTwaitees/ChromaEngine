#include "Mesh.h"


void Mesh::setupMesh()
{
	// Generate buffers
	// Vertex Array Object Buffer
	glGenVertexArrays(1, &VAO);
	// Vertex Buffer and Element Buffer
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex TexCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader)
{
	// updating shader's texture uniforms
	unsigned int diffuseNr{0};
	unsigned int specularNr{0};
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
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Vertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val)
{
	this->vertices = vertices_val;
	this->indices = indices_val;
	this->textures = textures_val;

	setupMesh();
}

Mesh::~Mesh()
{

}
