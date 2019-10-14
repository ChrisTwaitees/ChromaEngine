#include "StaticMesh.h"


void StaticMesh::setupMesh()
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

void StaticMesh::updateUniforms(Shader& updateShader, std::vector < std::shared_ptr<Light>> Lights, Camera& RenderCam, glm::mat4& TransformMatrix)
{
	updateTransformUniforms(updateShader, RenderCam, TransformMatrix);
	updateTextureUniforms(updateShader);
	updateMaterialUniforms(updateShader);
	updateLightingUniforms(updateShader, Lights, RenderCam);
}

void StaticMesh::updateLightingUniforms(Shader& shader, std::vector < std::shared_ptr<Light>> Lights, Camera& renderCam)
{
	int pointlights{ 0 };
	int dirlights{ 0 };
	int spotlights{ 0 };
	for (int i = 0; i < Lights.size(); i++)
	{
		std::string lightIndex;
		// set uniforms
		switch (Lights[i]->type) {
		case Light::POINT:
			pointlights++;
			lightIndex = "pointLights[" + std::to_string(pointlights - 1) + "]";
			break;
		case Light::SUNLIGHT :
		case Light::DIRECTIONAL :
			dirlights++;
			lightIndex = "dirLights[" + std::to_string(dirlights - 1) + "]";
			break;
		case Light::SPOT:
			spotlights++;
			lightIndex = "spotLights[" + std::to_string(spotlights - 1) + "]";
			break;
		default:
			break;
		}
		//// lights directional
		shader.setVec3(lightIndex + ".direction", Lights[i]->direction);
		shader.setVec3(lightIndex + ".position", Lights[i]->position);
		shader.setVec3(lightIndex + ".diffuse", Lights[i]->diffuse);
		shader.setFloat(lightIndex + ".intensity", Lights[i]->intensity);
		//// lights spotlight
		shader.setFloat(lightIndex + ".spotSize", Lights[i]->spotSize);
		shader.setFloat(lightIndex + ".penumbraSize", Lights[i]->penumbraSize);
		//// lights point light falloff
		shader.setFloat(lightIndex + ".constant", Lights[i]->constant);
		shader.setFloat(lightIndex + ".linear", Lights[i]->linear);
		shader.setFloat(lightIndex + ".quadratic", Lights[i]->quadratic);
		//// lights view pos
		shader.setVec3("viewPos", renderCam.get_position());
	}
}

void StaticMesh::updateTextureUniforms(Shader& shader)
{
	// updating shader's texture uniforms
	unsigned int diffuseNr{ 1 };
	unsigned int specularNr{ 1 };
	unsigned int shadowmapNr{ 1 };
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);// activate proper texture unit before binding
		// building the uniform name
		std::string name;
		std::string texturenum;
		if (textures[i].type == Texture::DIFFUSE)
		{
			name = "material.texture_diffuse";
			texturenum = std::to_string(diffuseNr++);
		}
		if (textures[i].type == Texture::SPECULAR)
		{
			name = "material.texture_specular";
			texturenum = std::to_string(specularNr++);
		}
		if (textures[i].type == Texture::SHADOWMAP)
		{
			name = "shadowmaps.shadowmap";
			texturenum = std::to_string(shadowmapNr++);
		}
		// setting uniform and binding texture
		shader.setInt(( name + texturenum).c_str(), i);

		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		// activate texture
	}
	glActiveTexture(GL_TEXTURE0);
}

void StaticMesh::updateTransformUniforms(Shader& shader, Camera& renderCam, glm::mat4& modelMatrix)
{

	glm::mat4 finalTransform = getTransformationMatrix() * modelMatrix;
	shader.setMat4("model", finalTransform);
	shader.setMat4("view", renderCam.viewMat);
	shader.setMat4("projection", renderCam.projectionMat);
}

void StaticMesh::updateMaterialUniforms(Shader& shader)
{
	shader.setFloat("material.ambientBrightness", 0.16f);
	shader.setFloat("material.roughness", 64.0f);
	shader.setFloat("material.specularIntensity", 1.0f);
	shader.setFloat("material.cubemapIntensity", 1.0f);
}

void StaticMesh::Draw(Shader &shader)
{
	shader.use();
	// draw mesh
	BindDrawVAO();
}

void StaticMesh::Draw(Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix)
{
	pShader->use();
	updateUniforms(*pShader, Lights, RenderCamera, transformMatrix);
	BindDrawVAO();
}

void StaticMesh::Draw(Shader& shader, Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix)
{
	shader.use();
	updateUniforms(shader, Lights, RenderCamera, transformMatrix);
	BindDrawVAO();
}

void StaticMesh::BindDrawVAO()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // reset to default
}


void StaticMesh::bindShader(Shader* newShader)
{
	pShader = newShader;
}

void StaticMesh::bindTextures(std::vector<Texture> textures_val)
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
			textures.push_back(textures_val[i]);
	}
}

void StaticMesh::bindTexture(Texture texture_val)
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

void StaticMesh::setMat4(std::string name, glm::mat4 value)
{
	pShader->setMat4(name, value);
}

void StaticMesh::setInt(std::string name, int value)
{
	pShader->setInt(name, value);
}

StaticMesh::StaticMesh(std::vector<Vertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val)
{
	isRenderable = true;
	vertices = vertices_val;
	indices = indices_val;
	textures = textures_val;
	

	setupMesh();
}

StaticMesh::StaticMesh()
{
	isRenderable = true;
}

StaticMesh::~StaticMesh()
{
	delete pShader;
}