#include "SkyBox.h"
#include <scene/Scene.h>

void SkyBox::Initialize()
{
	// initialize shaders uniforms
	m_linearShader.SetInt("skybox", 0);
	m_HDRShader.SetInt("skybox", 0);

	// create VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set VBO buffer data
	glBufferData(GL_ARRAY_BUFFER, skyBoxVerts.size() * sizeof(float), &skyBoxVerts[0], GL_STATIC_DRAW);

	// create and bindShadowMapToBuffer VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// set vertex attribpointers
	glEnableVertexAttribArray(0); // aPos for TexCoords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindVertexArray(0);
}

void SkyBox::Draw()
{
	// set view and projection matrix
	glm::mat4 view = glm::mat4(glm::mat3(Chroma::Scene::GetRenderCamera()->GetViewMatrix()));
	// shader
	switch (m_colorSpace)
	{
	case(LINEAR) :
	{
		m_linearShader.Use();
		m_linearShader.SetMat4("view", view);
		m_linearShader.SetMat4("projection", Chroma::Scene::GetRenderCamera()->GetProjectionMatrix());
		break;
	}
	case(HDR):
	{
		m_HDRShader.Use();
		m_HDRShader.SetMat4("view", view);
		m_HDRShader.SetMat4("projection", Chroma::Scene::GetRenderCamera()->GetProjectionMatrix());
		break;
	}
	}
	// texture 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapID);
	// change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);
	// draw vao
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	// set depth function back to default
	glDepthFunc(GL_LESS); 
}

SkyBox::SkyBox()
{
	//m_cubeMap = CubeMap(defaultImageDir);
	//m_cubeMapID = m_cubeMap.ID;
	Initialize();
}


SkyBox::~SkyBox()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
