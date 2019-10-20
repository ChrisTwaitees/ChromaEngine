#include "SkyBox.h"

void SkyBox::initialize()
{
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
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	// set view and projection matrix
	glm::mat4 view = glm::mat4(glm::mat3(activeCamera->viewMat));
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", activeCamera->projectionMat);

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.ShaderID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

SkyBox::SkyBox(Camera& activeCamera_val)
{

	cubeMap = CubeMap(defaultImageDir);
	activeCamera = &activeCamera_val;
	initialize();
}


SkyBox::~SkyBox()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
