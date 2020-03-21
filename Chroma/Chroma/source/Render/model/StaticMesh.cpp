#include "StaticMesh.h"
#include <scene/Scene.h>


void StaticMesh::CalculateBBox()
{
	// collect verts
	m_vertices = GetVertices();

	// calculate new min and max bbox
	glm::vec3 newMinBBox(99999.00, 99999.00, 99999.00);
	glm::vec3 newMaxBBox(0.0, 0.0, 0.0);

	for (ChromaVertex& vert : m_vertices)
	{
		newMinBBox = glm::min(newMinBBox, vert.m_position);
		newMaxBBox = glm::max(newMaxBBox, vert.m_position);
	}
	// re-establishing min and max bboxes
	m_BBoxMin = newMinBBox;
	m_BBoxMax = newMaxBBox;
}

void StaticMesh::CalculateCentroid()
{
	m_Centroid = (m_BBoxMin - m_BBoxMax) * glm::vec3(0.5);
}

void StaticMesh::SetupMesh()
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int),
		&m_Indices[0], GL_STATIC_DRAW);

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

	// BBOX
	CalculateBBox();
	CalculateCentroid();
}

void StaticMesh::UpdateUniforms(Shader& shader, Camera& RenderCam)
{
	UpdateTransformUniforms(shader, RenderCam);
	UpdateMaterialUniforms(shader);
	updateTextureUniforms(shader);
	UpdateLightingUniforms(shader, RenderCam);
}

void StaticMesh::UpdateLightingUniforms(Shader& shader, Camera& renderCam)
{
	shader.SetLightingUniforms(renderCam);
}

void StaticMesh::updateTextureUniforms(Shader const& shader)
{
	// UV Modifiers
	shader.SetVec2("UVMultiply", m_UVMultiply);

	// updating shader's texture uniforms
	unsigned int diffuseNr{ 1 };
	unsigned int shadowmapNr{ 1 };
	unsigned int normalNr{ 1 };
	unsigned int roughnessNr{ 1 };
	unsigned int metalnessNr{ 1 };
	unsigned int metroughaoNr{ 1 };
	unsigned int aoNr{ 1 };
	unsigned int translucencyNr{ 1 };
	for (int i = 0; i < m_Textures.size(); i++)
	{
		// building the uniform name
		std::string name;
		std::string texturenum;
		Texture::TYPE textureType = m_Textures[i].type;

		switch(textureType)
		{
		case Texture::ALBEDO :
			{
				name = "material.texture_albedo";
				texturenum = std::to_string(diffuseNr++);
				// set use texture albedo
				shader.SetBool("UseAlbedoMap", true);
				break;
			}
		case Texture::NORMAL:
			{
				name = "material.texture_normal";
				texturenum = std::to_string(normalNr++);
				// set use texture normals
				shader.SetBool("UseNormalMap", true);
				break;
			}
		case Texture::METROUGHAO:
			{
				name = "material.texture_MetRoughAO";
				texturenum = std::to_string(metroughaoNr++);
				// set use texture metroughao
				shader.SetBool("UseMetRoughAOMap", true);
				break;
			}
		case Texture::METALNESS:
			{
				name = "material.texture_metalness";
				texturenum = std::to_string(metalnessNr++);
				break;
			}
		case Texture::ROUGHNESS:
			{
				name = "material.texture_roughness";
				texturenum = std::to_string(roughnessNr++);
				break;
			}
		case Texture::AO:
			{
				name = "material.texture_ao";
				texturenum = std::to_string(aoNr++);
				break;
			}
		case Texture::SHADOWMAP:
			{	
				name = "shadowmaps.shadowmap";
				texturenum = std::to_string(shadowmapNr++);
				break;
			}
		case Texture::TRANSLUCENCY:
		{
			name = "material.texture_translucency";
			texturenum = std::to_string(translucencyNr++);
			shader.SetBool("UseTranslucencyMap", true);
			break;
		}
		}

		// Activate Texture before binding
		glActiveTexture(GL_TEXTURE0 + i);
		// Bind Texture
		glBindTexture(GL_TEXTURE_2D, m_Textures[i].ID);
		// Set Unitform
		shader.SetInt(( name + texturenum).c_str(), i);


	}

	if (m_IsForwardLit)
		UpdatePBRLightingTextureUniforms(shader);

	glActiveTexture(GL_TEXTURE0);

}

void StaticMesh::UpdatePBRLightingTextureUniforms(Shader const& shader)
{
	// Irradiance
	glActiveTexture(GL_TEXTURE0 + m_Textures.size() + 1);
	shader.SetInt("irradianceMap", m_Textures.size() + 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Chroma::Scene::GetIBL()->GetIrradianceMapID());
	// Prefilter Map
	glActiveTexture(GL_TEXTURE0 + m_Textures.size() + 2);
	shader.SetInt("prefilterMap", m_Textures.size() + 2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Chroma::Scene::GetIBL()->GetPrefilterMapID());
	// BRDF LUT
	glActiveTexture(GL_TEXTURE0 + m_Textures.size() + 3);
	shader.SetInt("brdfLUT", m_Textures.size() + 3);
	glBindTexture(GL_TEXTURE_2D, Chroma::Scene::GetIBL()->GetBRDFLUTID());
}

void StaticMesh::UpdateTransformUniforms(Shader const& shader, Camera& renderCam)
{
	shader.SetMat4("model", GetWorldTransform());
	shader.SetMat4("view", renderCam.GetViewMatrix());
	shader.SetMat4("projection", renderCam.GetProjectionMatrix());
}

void StaticMesh::UpdateMaterialUniforms(Shader const& shader)
{
	shader.SetFloat("roughness", 0.4f);
	shader.SetVec3("color", glm::vec4(1, 0, 0, 0.5));
	shader.SetFloat("metalness", 0.0f);
	shader.SetBool("UseAlbedoMap", false);
	shader.SetBool("UseNormalMap", false);
	shader.SetBool("UseMetRoughAOMap", false);
}

void StaticMesh::Draw(Shader& shader)
{
	shader.Use();
	// draw mesh
	BindDrawVAO();
}

void StaticMesh::Draw(Camera& RenderCamera)
{
	m_shader.Use();
	UpdateUniforms(m_shader, RenderCamera);
	BindDrawVAO();
}

void StaticMesh::Draw(Shader& shader, Camera& RenderCamera)
{
	shader.Use();
	UpdateUniforms(shader, RenderCamera);
	BindDrawVAO();
}

void StaticMesh::DrawUpdateMaterials(Shader& shader)
{
	shader.Use();
	UpdateMaterialUniforms(shader);
	updateTextureUniforms(shader);
	BindDrawVAO();
}

void StaticMesh::DrawUpdateTransforms(Camera& renderCam)
{
	m_shader.Use();
	UpdateTransformUniforms(m_shader, renderCam);
	BindDrawVAO();
}

void StaticMesh::BindDrawVAO()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // reset to default
}

void StaticMesh::Destroy()
{
	// textures
	for (Texture& texture : m_Textures)
	{
		texture.Destroy();
	}
	m_Textures.clear();
	// verts
	m_vertices.clear();
	// vao
	glDeleteVertexArrays(1, &VAO);
	// buffers
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	CHROMA_INFO("Static Mesh : {0} Destroyed.", m_UID.data);
}


void StaticMesh::SetTextures(std::vector<Texture> textures_val)
{
	for (unsigned int i = 0; textures_val.size(); i++)
	{
		bool skip{ false };
		for (unsigned int j = 0; j < m_Textures.size(); j++)
		{
			if (std::strcmp(m_Textures[j].GetSourcePath().data(), textures_val[j].GetSourcePath().data()) == 0)
			{
				skip = true;
				break;
			}
		}
		if (!skip)
			m_Textures.push_back(textures_val[i]);
	}
}

void StaticMesh::AddTexture(Texture texture_val)
{
	bool skip{false};
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		skip = false;
		if (std::strcmp(m_Textures[i].GetSourcePath().data(), texture_val.GetSourcePath().data()) == 0)
		{
			skip = true;
			break;
		}
	}
	if (!skip)
	{
		m_Textures.push_back(texture_val);
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
	m_shader.Use();
	m_shader.SetMat4(name, value);
}

void StaticMesh::SetInt(std::string name, int value)
{
	m_shader.Use();
	m_shader.SetInt(name, value);
}

void StaticMesh::SetFloat(std::string name, float value)
{
	m_shader.Use();
	m_shader.SetFloat(name, value);
}

StaticMesh::StaticMesh(std::vector<ChromaVertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val)
{
	m_IsRenderable = true;
	m_vertices = vertices_val;
	m_Indices = indices_val;
	m_Textures = textures_val;
	
	SetupMesh();
}

StaticMesh::StaticMesh(MeshData const& newMeshData)
{
	m_IsRenderable = true;
	m_vertices = newMeshData.verts;
	m_Indices = newMeshData.indices;
	m_Textures = newMeshData.textures;

	SetupMesh();
}

StaticMesh::StaticMesh()
{
	m_IsRenderable = true;
}

StaticMesh::~StaticMesh()
{
}
