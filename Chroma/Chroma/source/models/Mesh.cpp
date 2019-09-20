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

void Mesh::Draw(Shader &shader)
{
	// updating shader's texture uniforms
	unsigned int diffuseNr{1};
	unsigned int specularNr{1};
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);// activate proper texture unit before binding
		// building the uniform name
		std::string name;
		std::string texturenum;
		if (textures[i].type == Texture::DIFFUSE)
		{
			name = "texture_diffuse";
			texturenum = std::to_string(diffuseNr++);
		}
		if (textures[i].type == Texture::SPECULAR)
		{
			name = "texture_specular";
			texturenum = std::to_string(specularNr++);
		}
		std::cout << "diffuse texture: " << textures[i].path << " attempting to draw." << std::endl;
		// setting uniform and binding texture
		shader.setInt(("material." + name + texturenum).c_str(), i);

		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		// activate texture
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::bindTextures(std::vector<Texture> textures_val)
{
	for (unsigned int i = 0; textures_val.size(); i++)
	{
		bool skip{ false };
		for (unsigned int j = 0; j < textures.size(); j++)
		{
			if (std::strcmp(textures[j].path.data(), textures_val[j].path.data()) == 0)
			{
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			textures.push_back(textures_val[i]);
		}
	}
}

void Mesh::bindTexture(Texture texture_val)
{
	bool skip{false};
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		skip = false;
		if (std::strcmp(textures[i].path.data(), texture_val.path.data()) == 0)
		{
			skip = true;
			break;
		}
	}
	if (!skip)
	{
		textures.push_back(texture_val);
	}
}

Mesh::Mesh(std::vector<Vertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val)
{
	this->vertices = vertices_val;
	this->indices = indices_val;
	this->textures = textures_val;

	setupMesh();
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{

}
