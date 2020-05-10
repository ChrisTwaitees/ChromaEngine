#ifndef CHROMA_UNIFORMBUFFER_LIGHTING_H
#define CHROMA_UNIFORMBUFFER_LIGHTING_H
#include <ubo/UniformBuffer.h>

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
	DirLight m_DirLightStrucs[MAX_DIRECTIONAL_LIGHTS];
	PointLight m_PointLightStructs[MAX_POINT_LIGHTS];
	SpotLight m_SpotLightStructs[MAX_SPOT_LIGHTS];

};

#endif // CHROMA_UNIFORMBUFFER_LIGHTING_H