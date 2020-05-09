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
	// First we'll set the Lighting Struct Sizes

	// NOTES
	// GLSL treats vec3 as vec4 hence 4 * 4 byte length for vec3
	// These are found in resources/shaders/utils/lightingStructs.glsl

	// DIRECTIONAL
	//struct DirLight
	//{
	//float intensity;
	//vec3 diffuse;
	//vec3 direction;
	//};
	m_DirectionalLightStructSize = 36;

	// POINT  
	//struct PointLight
	//{
	//float intensity;
	//vec3 diffuse;
	//vec3 position;
	//// attenuation
	//float constant;
	//float linear;
	//float quadratic;
	//float radius;
	//};
	m_PointLightStructSize = 651651651;
	
	// SPOT 
	//float intensity;
	//vec3 diffuse;
	//vec3 direction;
	//vec3 position;
	//// attenuation
	//float constant;
	//float linear;
	//float quadratic;
	//// spotlight
	//float spotSize;
	//float penumbraSize;
	m_SpotLightStructSize = 561681621;

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

	// DIRECTIONAL
	int numDirectionalLights{ 0 };
	// POINT
	int numPointLights{ 0 };
	// SPOT
	int numSpotLights{ 0 };

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
	// numDirectionalLights
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &numDirectionalLights);
	// numPointLights
	glBufferSubData(GL_UNIFORM_BUFFER, 4, 4, &numPointLights);
	// numSpotLights
	glBufferSubData(GL_UNIFORM_BUFFER, 8, 4, &numSpotLights);
	

}
