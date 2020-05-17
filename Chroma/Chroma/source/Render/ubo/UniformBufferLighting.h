#ifndef CHROMA_UNIFORMBUFFER_LIGHTING_H
#define CHROMA_UNIFORMBUFFER_LIGHTING_H
#include <ubo/UniformBuffer.h>

struct DirLight
{
	glm::vec4 diffuse;
	glm::vec3 direction;
	float intensity;
};

struct PointLight
{
	glm::vec4 diffuse;
	glm::vec3 position;
	float intensity;
	float constant;
	float linear;
	float quadratic;
	float radius;
};

struct SpotLight
{
	glm::vec4 diffuse;
	glm::vec3 direction;
	float intensity;
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	float spotSize;
	float penumbraSize;
};


class UniformBufferLighting : public UniformBuffer
{
public:

	UniformBufferLighting() { m_Name = "LightingUBO"; 	Setup(); };
	~UniformBufferLighting() {};

	void Update() override;

private:

	void Setup() override;
	void PopulateBufferWithSceneLights();

	// Lighting Struct Sizes
	DirLight m_DirLightStructs[MAX_DIRECTIONAL_LIGHTS];
	PointLight m_PointLightStructs[MAX_POINT_LIGHTS];
	SpotLight m_SpotLightStructs[MAX_SPOT_LIGHTS];

};

#endif // CHROMA_UNIFORMBUFFER_LIGHTING_H