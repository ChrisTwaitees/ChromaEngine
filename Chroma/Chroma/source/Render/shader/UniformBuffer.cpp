
#include "UniformBuffer.h"
#include <scene/Scene.h>
#include <render/Render.h>

unsigned int UniformBuffer::UBO_GlobalBindingPointIndex{ 0 };

void UniformBuffer::BindUniformBlockIndex(const Shader& shaderToBind)
{
	// But, how does OpenGL know what uniform buffers correspond to which uniform blocks ?
	// We need to link the ubo to a binding point that all other shaders are aware of 
	// we need to link both the shader and the ubo to the same block binding point in order for them 
	// to be aware of them we use glUniformBlockBinding 
	// This has to be done for each shader
	unsigned int uniformBufferBlockIndex = glGetUniformBlockIndex(shaderToBind.ShaderID, m_Name.c_str());
	glUniformBlockBinding(shaderToBind.ShaderID, uniformBufferBlockIndex, m_BindingPointIndex);

}

void UniformBuffer::AppendVec3(const glm::vec3& newVec3)
{
	// To update the UBO bind the UBO and use glBufferSubData
	Bind();
	// We only have to update this uniform buffer once, and all shaders that use this buffer now use its updated data
	// We can add data via a single byte array or up[date parts of the buffer whenever we feel like it using 
	// glBufferSubData
	int b = true; // bools in glsl are represedned as 4 bytes, so we store it in an integer
	glBufferSubData(GL_UNIFORM_BUFFER, 144, 4, &b);
	
	UnBind();


}

void UniformBuffer::Update()
{
	Bind();
	// projection
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(Chroma::Scene::GetRenderCamera()->GetProjectionMatrix()));
	// view
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Chroma::Scene::GetRenderCamera()->GetViewMatrix()));
	UnBind();
}

void UniformBuffer::Initialize()
{
	UBO_GlobalBindingPointIndex++;
	m_BindingPointIndex = UBO_GlobalBindingPointIndex;
	// Create the Uniform Buffer Object
	glGenBuffers(1, &m_UBO);
	// Bind and allocate memory
	Bind();
	m_Size = 2 * sizeof(glm::mat4); // view , projection and lightspace matrices
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
	//// lightspacematrix
	//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, sizeof(glm::mat4), glm::value_ptr(static_cast<ShadowBuffer*>(Chroma::Render::GetShadowBuffer())->GetLightSpaceMatrix()));
	UnBind();

}
