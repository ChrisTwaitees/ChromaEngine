#include "Icon.h"
#include <resources/ResourceManager.h>
#include <scene/Scene.h>

void Icon::SetIconTexture(std::string const& iconPath)
{
	m_IconTexture.Destroy();
	m_IconSourcePath = iconPath;
	m_IconTexture = Chroma::ResourceManager::LoadTexture(m_IconSourcePath);
}

void Icon::SetIconTexture(Texture& newTexture)
{
	m_IconTexture.Destroy();
	m_IconSourcePath = newTexture.GetSourcePath();
	m_IconTexture = newTexture;
}

void Icon::Draw()
{
	// use shader
	m_IconShader.Use();

	// use icons texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_IconTexture.ID);
	m_IconShader.SetUniform("Texture", 0);

	// Transforms
	m_IconShader.SetUniform("projection", Chroma::Scene::GetRenderCamera()->GetProjectionMatrix());
	Chroma::Math::TransposeViewToModelMatrixParticles(m_ModelMatrix, Chroma::Scene::GetRenderCamera()->GetViewMatrix());
	m_IconShader.SetUniform("modelView", Chroma::Scene::GetRenderCamera()->GetViewMatrix() * m_ModelMatrix);
	// Draw
	BindDrawVAO();
}

Icon::Icon(std::string const& iconPath)
{
	m_IconSourcePath = iconPath;
	m_IconTexture = Chroma::ResourceManager::LoadTexture(m_IconSourcePath);

	Initialize();
}

Icon::Icon()
{
	Initialize();
}


Icon::~Icon()
{
}

void Icon::Initialize()
{
	SetupQuad();
	UpdateTransform();
}

void Icon::UpdateTransform()
{
	m_ModelMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale));
	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
}

void Icon::SetupQuad()
{
	// positions
	glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3(1.0f, -1.0f, 0.0f);
	glm::vec3 pos4(1.0f, 1.0f, 0.0f);
	// texture coordinates
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);
	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);

	float quadVertices[] = {
		// positions            // normal         // texcoords 
		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y,
		pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y,
															  
		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y,
		pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y
	};
	// configure plane VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}

void Icon::BindDrawVAO()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0); // reset to default
}
