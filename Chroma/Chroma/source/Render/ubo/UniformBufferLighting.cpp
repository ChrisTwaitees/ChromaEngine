#include "UniformBufferLighting.h"
#include <scene/Scene.h>

void UniformBufferLighting::Update()
{
	//// Bind and allocate memory
	//Bind();
	//// Calculate the current number of lights and structs needed
	//m_Size = CalculateBufferSize(); // view , projection and lightspace matrices
	//glBufferData(GL_UNIFORM_BUFFER, m_Size, NULL, GL_DYNAMIC_DRAW);
	//// Now the buffer has been resized we'll repopulate it
	//PopulateBufferWithSceneLights();
	//UnBind();
}

void UniformBufferLighting::Setup()
{
	// Bind and allocate memory
	Bind();
	// Calculate the current number of lights and structs needed
	m_Size = CalculateBufferSize();
	glBufferData(GL_UNIFORM_BUFFER, m_Size, NULL, GL_DYNAMIC_DRAW);
	// Set to default uniform buffer
	UnBind();

	// Then we need to bind the uniform buffer object to the same binding point 
	// We've bound the shaders to
	glBindBufferRange(GL_UNIFORM_BUFFER, m_BindingPointIndex, m_UBO, 0, m_Size);

	// Now we've created a buffer with enough space we'll fill the buffer
	Bind();
	PopulateBufferWithSceneLights();
	UnBind();

	// Debug
	CHROMA_INFO("Uniform Buffer Object : {} Setup.", m_Name);
}

unsigned int UniformBufferLighting::CalculateBufferSize()
{
	for (UID const& lightUID : Chroma::Scene::GetLightUIDs())
	{
		CHROMA_INFO("LightIntensity : {}", static_cast<Light*>(Chroma::Scene::GetComponent(lightUID))->GetIntensity());
	}
	return 16;
}

void UniformBufferLighting::PopulateBufferWithSceneLights()
{
	for (UID const& lightUID : Chroma::Scene::GetLightUIDs())
	{

	}

	// projection
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(Chroma::Scene::GetRenderCamera()->GetProjectionMatrix()));
	// view
	//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Chroma::Scene::GetRenderCamera()->GetViewMatrix()));
	//// lightspacematrix
	//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, sizeof(glm::mat4), glm::value_ptr(static_cast<ShadowBuffer*>(Chroma::Render::GetShadowBuffer())->GetLightSpaceMatrix()));

}
