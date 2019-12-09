#include "StaticMesh.h"


void StaticMesh::CalculateBBox()
{
	m_vertices = GetVertices();
	// collecting all bboxes within mesh components of entity and returning overall
	std::vector<std::pair<glm::vec3, glm::vec3>> bboxes;
	// once collected, calculate new min and max bbox
	glm::vec3 newMinBBox(99999.00, 99999.00, 99999.00);
	glm::vec3 newMaxBBox(0.0, 0.0, 0.0);
	for (ChromaVertex& vert : m_vertices)
	{
		newMinBBox = glm::min(newMinBBox, vert.GetPosition());
		newMaxBBox = glm::max(newMaxBBox, vert.GetPosition());
	}
	// re-establishing min and max bboxes
	m_BBoxMin = newMinBBox;
	m_BBoxMax = newMaxBBox;
}

void StaticMesh::CalculateCentroid()
{
	m_Centroid = (m_BBoxMin - m_BBoxMax) * glm::vec3(0.5);
}

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
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(ChromaVertex), &m_vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
		&m_indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)offsetof(ChromaVertex, ChromaVertex::m_normal));
	// vertex uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)offsetof(ChromaVertex, ChromaVertex::m_texCoords));
	// vertex tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)offsetof(ChromaVertex, ChromaVertex::m_tangent));
	// vertex bitangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)offsetof(ChromaVertex, ChromaVertex::m_bitangent));

	glBindVertexArray(0);
}

void StaticMesh::updateUniforms(const Shader* updateShader, std::vector<Light*> Lights, Camera& RenderCam, glm::mat4& TransformMatrix)
{
	updateTransformUniforms(updateShader, RenderCam, TransformMatrix);
	updateMaterialUniforms(updateShader);
	updateTextureUniforms(updateShader);
	updateLightingUniforms(updateShader, Lights, RenderCam);
}

void StaticMesh::updateLightingUniforms(const Shader* shader, std::vector<Light*> Lights, Camera& renderCam)
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
		shader->setVec3(lightIndex + ".direction", Lights[i]->GetDirection());
		shader->setVec3(lightIndex + ".position", Lights[i]->GetPosition());
		shader->setVec3(lightIndex + ".diffuse", Lights[i]->getDiffuse());
		shader->SetFloat(lightIndex + ".intensity", Lights[i]->getIntensity());
		//// lights spotlight
		shader->SetFloat(lightIndex + ".spotSize", Lights[i]->getSpotSize());
		shader->SetFloat(lightIndex + ".penumbraSize", Lights[i]->getPenumbraSize());
		//// lights point light falloff
		shader->SetFloat(lightIndex + ".constant", Lights[i]->constant);
		shader->SetFloat(lightIndex + ".linear", Lights[i]->linear);
		shader->SetFloat(lightIndex + ".quadratic", Lights[i]->quadratic);
		shader->SetFloat(lightIndex + ".radius", Lights[i]->getRadius());
		//// lights view pos
		shader->setVec3("viewPos", renderCam.GetPosition());
	}
}

void StaticMesh::updateTextureUniforms(const Shader* shader)
{
	// updating shader's texture uniforms
	unsigned int diffuseNr{ 1 };
	unsigned int shadowmapNr{ 1 };
	unsigned int normalNr{ 1 };
	unsigned int roughnessNr{ 1 };
	unsigned int metalnessNr{ 1 };
	unsigned int metroughaoNr{ 1 };
	unsigned int aoNr{ 1 };
	for (int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);// activate proper texture unit before binding
		// building the uniform name
		std::string name;
		std::string texturenum;
		if (m_textures[i].type == Texture::ALBEDO)
		{
			name = "material.texture_albedo";
			texturenum = std::to_string(diffuseNr++);
			// set use texture albedo
			shader->setBool("UseAlbedoMap", true);
		}
		if (m_textures[i].type == Texture::NORMAL)
		{
			name = "material.texture_normal";
			texturenum = std::to_string(normalNr++);
			// set use texture normals
			shader->setBool("UseNormalMap", true);
		}
		if (m_textures[i].type == Texture::METROUGHAO)
		{
			name = "material.texture_MetRoughAO";
			texturenum = std::to_string(metroughaoNr++);
			// set use texture metroughao
			shader->setBool("UseMetRoughAOMap", true);
		}
		if (m_textures[i].type == Texture::METALNESS)
		{
			name = "material.texture_metalness";
			texturenum = std::to_string(metalnessNr++);
		}
		if (m_textures[i].type == Texture::ROUGHNESS)
		{
			name = "material.texture_roughness";
			texturenum = std::to_string(roughnessNr++);
		}
		if (m_textures[i].type == Texture::AO)
		{
			name = "material.texture_ao";
			texturenum = std::to_string(aoNr++);
		}
		if (m_textures[i].type == Texture::SHADOWMAP)
		{
			name = "shadowmaps.shadowmap";
			texturenum = std::to_string(shadowmapNr++);
		}

		// setting uniform and binding texture
		shader->SetInt(( name + texturenum).c_str(), i);

		glBindTexture(GL_TEXTURE_2D, m_textures[i].ID);
		// activate texture
	}
	glActiveTexture(GL_TEXTURE0);
}

void StaticMesh::updateTransformUniforms(const Shader* shader, Camera& renderCam, glm::mat4& modelMatrix)
{

	glm::mat4 finalTransform = GetTransformationMatrix() * modelMatrix;
	shader->SetMat4("model", finalTransform);
	shader->SetMat4("view", renderCam.GetViewMatrix());
	shader->SetMat4("projection", renderCam.GetProjectionMatrix());
}

void StaticMesh::updateMaterialUniforms(const Shader* shader)
{
	shader->SetFloat("roughness", 0.4f);
	shader->setVec3("color", glm::vec4(1, 0, 0, 0.5));
	shader->SetFloat("metalness", 0.0f);
	shader->setBool("UseAlbedoMap", false);
	shader->setBool("UseNormalMap", false);
	shader->setBool("UseMetRoughAOMap", false);
}

void StaticMesh::Draw(Shader &shader)
{
	shader.use();
	// draw mesh
	BindDrawVAO();
}

void StaticMesh::Draw(Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix)
{
	m_shader->use();
	updateUniforms(m_shader, Lights, RenderCamera, transformMatrix);
	BindDrawVAO();
}

void StaticMesh::Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix)
{
	shader.use();
	updateUniforms(&shader, Lights, RenderCamera, transformMatrix);
	BindDrawVAO();
}

void StaticMesh::DrawUpdateMaterials(Shader& shader)
{
	shader.use();
	updateMaterialUniforms(&shader);
	updateTextureUniforms(&shader);
	BindDrawVAO();
}

void StaticMesh::DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix)
{
	m_shader->use();
	updateTransformUniforms(m_shader, renderCam, modelMatrix);
	BindDrawVAO();
}

void StaticMesh::BindDrawVAO()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // reset to default
}


void StaticMesh::SetShader(Shader* const& newShader)
{
	m_shader = newShader;
}

void StaticMesh::SetTextures(std::vector<Texture> textures_val)
{
	for (unsigned int i = 0; textures_val.size(); i++)
	{
		bool skip{ false };
		for (unsigned int j = 0; j < m_textures.size(); j++)
		{
			if (std::strcmp(m_textures[j].path.data(), textures_val[j].path.data()) == 0)
			{
				skip = true;
				break;
			}
		}
		if (!skip)
			m_textures.push_back(textures_val[i]);
	}
}

void StaticMesh::AddTexture(Texture texture_val)
{
	bool skip{false};
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		skip = false;
		if (std::strcmp(m_textures[i].path.data(), texture_val.path.data()) == 0)
		{
			skip = true;
			break;
		}
	}
	if (!skip)
	{
		m_textures.push_back(texture_val);
	}
}


std::pair<glm::vec3, glm::vec3> StaticMesh::GetBBox()
{
	CalculateBBox();
	return std::make_pair(m_BBoxMin, m_BBoxMax);
}

glm::vec3 StaticMesh::GetCentroid()
{
	CalculateCentroid();
	return m_Centroid;
}

void StaticMesh::SetMat4(std::string name, glm::mat4 value)
{
	m_shader->use();
	m_shader->SetMat4(name, value);
}

void StaticMesh::SetInt(std::string name, int value)
{
	m_shader->use();
	m_shader->SetInt(name, value);
}

void StaticMesh::SetFloat(std::string name, float value)
{
	m_shader->use();
	m_shader->SetFloat(name, value);
}

StaticMesh::StaticMesh(std::vector<ChromaVertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val)
{
	m_IsRenderable = true;
	m_vertices = vertices_val;
	m_indices = indices_val;
	m_textures = textures_val;
	
	setupMesh();
}

StaticMesh::StaticMesh()
{
	m_IsRenderable = true;
}

StaticMesh::~StaticMesh()
{
	delete m_shader;
}
