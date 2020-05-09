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
	unsigned int CalculateBufferSize();
	void PopulateBufferWithSceneLights();
};

#endif // CHROMA_UNIFORMBUFFER_LIGHTING_H