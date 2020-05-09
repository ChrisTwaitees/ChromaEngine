#include "UniformBufferLighting.h"
#include <scene/Scene.h>

void UniformBufferLighting::Update()
{
	Bind();
	PopulateBufferWithSceneLights();
	UnBind();
}

void UniformBufferLighting::Setup()
{
	// Now we've created a buffer we'll set the size according to our 
	// structs and number of lights, populating the buffer 
	Bind();
	PopulateBufferWithSceneLights();
	// Then we need to bind the uniform buffer object to the same binding point 
	// We've bound the shaders to
	glBindBufferRange(GL_UNIFORM_BUFFER, m_BindingPointIndex, m_UBO, 0, m_Size);
	UnBind();

	// Debug
	CHROMA_INFO("Uniform Buffer Object : {} Setup.", m_Name);
}


void UniformBufferLighting::PopulateBufferWithSceneLights()
{
	// Calculate the current number of lights and structs needed
	m_Size = 0;
	// DIRECTIONAL
	int numDirectionalLights{ 0 };
	m_Size += sizeof(int);
	// POINT
	int numPointLights{ 0 };
	m_Size += sizeof(int);
	// SPOT
	int numSpotLights{ 0 };
	m_Size += sizeof(int);
	m_Size += sizeof(int);

	int test1 = sizeof(int);
	int test2 = sizeof(float);
	int test3 = sizeof(glm::vec4);
	int test4 = sizeof(glm::vec3);

	DirLight tests[3];

	tests[0].diffuse = glm::vec4(1.0,0.0, 0.0, 0.5);
	tests[0].direction = glm::vec4(0.0, 1.0, 0.0, 0.5);
	tests[0].intensity = glm::abs(glm::sin(GAMETIME));
	tests[1].diffuse = glm::vec4(0.0,0.0, 1.0, 0.5);
	tests[1].direction = glm::vec4(1.0, 0.0, 1.0, 0.5);
	tests[1].intensity = glm::abs(0.25);
	m_Size += sizeof(tests);


	for (UID const& lightUID : Chroma::Scene::GetLightUIDs())
	{
		Light* currentLight = static_cast<Light*>(Chroma::Scene::GetComponent(lightUID));
		switch (currentLight->type)
		{
		case(Light::DIRECTIONAL):
		{
			numDirectionalLights++;
			break;
		}
		case(Light::SUNLIGHT):
		{
			numDirectionalLights++;
			//test.diffuse = glm::vec4(currentLight->GetDiffuse(), 1.0);
			//test.diffuse = currentLight->GetDiffuse();
			//test.intensity = currentLight->GetIntensity() *0.5 ;
			//test.direction = glm::vec4(currentLight->GetDirection(), 1.0);
			break;
		}
		case(Light::POINT):
		{
			numPointLights++;
			break;
		}
		case(Light::SPOT):
		{
			numSpotLights++;
			break;
		}
		default:
		{
			CHROMA_ERROR("Light Type {0} Not Supported!", currentLight->type);
			break;
		}
		}
	}
	

	// With the size updated, we'll need to resize the buffer data 
	// and the Buffer Bind Attach Point for the shaders.
	glBufferData(GL_UNIFORM_BUFFER, m_Size, NULL, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, m_BindingPointIndex, m_UBO, 0, m_Size);

	// Now with the GPU memory allocated we need to fill the data
	// cameraPosition
	// numDirectionalLights
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &numDirectionalLights);
	// numPointLights
	glBufferSubData(GL_UNIFORM_BUFFER, 4, 4, &numPointLights);
	// numSpotLights
	glBufferSubData(GL_UNIFORM_BUFFER, 8, 4, &numSpotLights);
	glBufferSubData(GL_UNIFORM_BUFFER, 12, 4, &numSpotLights);
	// structs
	// DirectionalLightStructs
	glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(tests), &tests);

	/*glBufferSubData(GL_UNIFORM_BUFFER, 28, 12, &test.diffuse);
	glBufferSubData(GL_UNIFORM_BUFFER, 44, 16, &test.direction);*/
	
}
