#ifndef CHROMA_UNIFORMBUFFER_CAMERA_H
#define CHROMA_UNIFORMBUFFER_CAMERA_H


#include <ubo/UniformBuffer.h>

class UniformBufferCamera : public UniformBuffer
{
public:

	UniformBufferCamera() { m_Name = "CameraMatrices"; 	Setup();};
	~UniformBufferCamera() {};

	void Update() override;

private:

	void Setup() override;


};


#endif// CHROMA_UNIFORMBUFFER_CAMERA_H
