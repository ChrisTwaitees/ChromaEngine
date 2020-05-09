#include "UniformBufferCamera.h"
#include <scene/Scene.h>

void UniformBufferCamera::Update()
{
	Bind();
	// projection
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(Chroma::Scene::GetRenderCamera()->GetProjectionMatrix()));
	// view
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Chroma::Scene::GetRenderCamera()->GetViewMatrix()));
	// cameraPosition
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::vec3), &Chroma::Scene::GetRenderCamera()->GetPosition());
	UnBind();
}

void UniformBufferCamera::Setup()
{
	// Bind and allocate memory
	Bind();
	m_Size = 2 * sizeof(glm::mat4); // view , projection matrices
	m_Size += sizeof(glm::vec3); // camera position
	//glBufferData(GL_UNIFORM_BUFFER, m_Size, NULL, GL_STATIC_DRAW); // allocate m_Size bytes of memory
	glBufferData(GL_UNIFORM_BUFFER, m_Size, NULL, GL_DYNAMIC_DRAW);
	// Set to default uniform buffer
	UnBind();

	// We also ned to bind the uniform buffer object to the same binding point
	//glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingPointIndex, m_UBO);
	glBindBufferRange(GL_UNIFORM_BUFFER, m_BindingPointIndex, m_UBO, 0, m_Size);

	// Now we've created a buffer with enough space we'll fill the buffer
	Bind();
	// projection
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(Chroma::Scene::GetRenderCamera()->GetProjectionMatrix()));
	// view
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Chroma::Scene::GetRenderCamera()->GetViewMatrix()));
	// cameraPosition
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, sizeof(glm::vec3), &Chroma::Scene::GetRenderCamera()->GetPosition());
	UnBind();
	// Debug
	CHROMA_INFO("Uniform Buffer Object : {} Setup.", m_Name);
}
