
#include "UniformBuffer.h"


unsigned int UniformBuffer::UBO_GlobalBindingPointIndex{ 0 };

void UniformBuffer::Initialize()
{
	UBO_GlobalBindingPointIndex++;
	m_BindingPointIndex = UBO_GlobalBindingPointIndex;
	// Create the Uniform Buffer Object
	glGenBuffers(1, &m_UBO);
	CHROMA_INFO("Uniform Buffer Object ID : {} Initialized.", m_UBO);

}

void UniformBuffer::BindUniformBlockIndex(const Shader& shaderToBind)
{
	// Shaders need to be aware of their Uniform Buffer Block Binding Index to read from
	// We need to bind each shader to their respective UBO index
	unsigned int uniformBufferBlockIndex = glGetUniformBlockIndex(shaderToBind.ShaderID, m_Name.c_str());
	glUniformBlockBinding(shaderToBind.ShaderID, uniformBufferBlockIndex, m_BindingPointIndex);
}



