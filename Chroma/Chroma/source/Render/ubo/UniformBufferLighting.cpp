#include "UniformBufferLighting.h"
#include <scene/Scene.h>

namespace Chroma
{
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
		// Calculate the current number of lights and structs needed
		m_Size = 0;
		// Directional Lights (num dir lights (int) and structs)
		m_Size += sizeof(int);
		m_Size += sizeof(m_DirLightStructs);
		// Point Lights (num point lights (int) and structs)
		m_Size += sizeof(int);
		m_Size += sizeof(m_PointLightStructs);
		// Spot Lights (num spot lights (int) and structs) and padding for latout 140 which requires vec4 spacing between data 
		m_Size += sizeof(int);
		m_Size += sizeof(int);
		m_Size += sizeof(m_SpotLightStructs);
		// With the size updated, we'll need to resize the buffer data 	
		glBufferData(GL_UNIFORM_BUFFER, m_Size, NULL, GL_DYNAMIC_DRAW);
		// Then we need to bind the uniform buffer object to the same binding point 
		// We've bound the shaders to
		glBindBufferRange(GL_UNIFORM_BUFFER, m_BindingPointIndex, m_UBO, 0, m_Size);
		// Attach current lights to buffer
		PopulateBufferWithSceneLights();
		UnBind();

		// Debug
		CHROMA_INFO("Uniform Buffer Object : {} Setup.", m_Name);
	}


	void UniformBufferLighting::PopulateBufferWithSceneLights()
	{

		// DIRECTIONAL
		int numDirectionalLights{ 0 };

		// POINT
		int numPointLights{ 0 };

		// SPOT
		int numSpotLights{ 0 };
	
		// Iter through lights populating light structs
		for (UID const& lightUID : Chroma::Scene::GetLightUIDs())
		{
			Light* currentLight = static_cast<Light*>(Chroma::Scene::GetComponent(lightUID));
			switch (currentLight->GetLightType())
			{
			case(Chroma::Type::Light::kSunlight):
			{
				m_DirLightStructs[numDirectionalLights].diffuse = glm::vec4(currentLight->GetDiffuse(), 1.0);
				m_DirLightStructs[numDirectionalLights].direction = glm::vec4(currentLight->GetDirection(), 1.0);
				m_DirLightStructs[numDirectionalLights].intensity = currentLight->GetIntensity() ;
				numDirectionalLights++;
				break;
			}
			case(Chroma::Type::Light::kDirectionalLight):
			{
				m_DirLightStructs[numDirectionalLights].diffuse = glm::vec4(currentLight->GetDiffuse(), 1.0);
				m_DirLightStructs[numDirectionalLights].direction = glm::vec4(currentLight->GetDirection(), 1.0);
				m_DirLightStructs[numDirectionalLights].intensity = currentLight->GetIntensity();
				numDirectionalLights++;
				break;
			}
			case(Chroma::Type::Light::kPointLight):
			{
				m_PointLightStructs[numPointLights].diffuse = glm::vec4(currentLight->GetDiffuse(), 1.0);
				m_PointLightStructs[numPointLights].position = currentLight->GetPosition();
				m_PointLightStructs[numPointLights].intensity = currentLight->GetIntensity();
				m_PointLightStructs[numPointLights].constant = currentLight->GetConstant();
				m_PointLightStructs[numPointLights].linear = currentLight->GetLinear();
				m_PointLightStructs[numPointLights].quadratic = currentLight->GetQuadratic();
				m_PointLightStructs[numPointLights].radius = currentLight->GetRadius();
				numPointLights++;
				break;
			}
			case(Chroma::Type::Light::kSpotLight):
			{
				m_SpotLightStructs[numSpotLights].diffuse = glm::vec4(currentLight->GetDiffuse(), 1.0);
				m_SpotLightStructs[numSpotLights].direction = glm::vec4(currentLight->GetDirection(), 1.0);
				m_SpotLightStructs[numSpotLights].intensity = currentLight->GetIntensity();
				m_SpotLightStructs[numSpotLights].position = currentLight->GetPosition();
				m_SpotLightStructs[numSpotLights].constant = currentLight->GetConstant();
				m_SpotLightStructs[numSpotLights].linear = currentLight->GetLinear();
				m_SpotLightStructs[numSpotLights].quadratic = currentLight->GetQuadratic();
				m_SpotLightStructs[numSpotLights].spotSize = currentLight->GetSpotSize();
				m_SpotLightStructs[numSpotLights].penumbraSize = currentLight->GetPenumbraSize();
				numSpotLights++;
				break;
			}
			default:
			{
				CHROMA_ERROR("Light Type {0} Not Supported!", currentLight->GetLightType());
				break;
			}
			}
		}
	
		// numDirectionalLights
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &numDirectionalLights);
		// numPointLights
		glBufferSubData(GL_UNIFORM_BUFFER, 4, 4, &numPointLights);
		// numSpotLights
		glBufferSubData(GL_UNIFORM_BUFFER, 8, 4, &numSpotLights);
		glBufferSubData(GL_UNIFORM_BUFFER, 12, 4, &numSpotLights); //  Padding for lighting structs
		// structs
		// DirectionalLightStructs
		glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(m_DirLightStructs), &m_DirLightStructs);
		// PointLightStructs
		glBufferSubData(GL_UNIFORM_BUFFER, 16 + sizeof(m_DirLightStructs), sizeof(m_PointLightStructs), &m_PointLightStructs);
		// PointLightStructs
		glBufferSubData(GL_UNIFORM_BUFFER, 16 + sizeof(m_DirLightStructs) + sizeof(m_PointLightStructs), sizeof(m_SpotLightStructs), &m_SpotLightStructs);

	
	}
}
