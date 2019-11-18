#include "DebugBuffer.h"

void DebugBuffer::initialize()
{
	// build Line VAO
	generateLineVAO();
}

void DebugBuffer::blitDepthBuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_postFXBuffer->getFBO()); // fetch depth from postFXBuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); // copy depth to debug buffer
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
}

void DebugBuffer::generateLineVAO()
{
	ChromaVertex singleVert;

	// Generate buffers
	// Vertex Array Object Buffer
	glGenVertexArrays(1, &lineVAO);
	// Vertex Buffer and Element Buffer
	glGenBuffers(1, &lineVBO);

	// Bind buffers
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ChromaVertex), &singleVert, GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)0);

}

void DebugBuffer::GenTexture()
{
	glGenTextures(1, &FBOTexture);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_HEIGHT, SCREEN_WIDTH, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

void DebugBuffer::drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color)
{
	attachBuffer();
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	m_debugLineShader.use();
	m_debugLineShader.setVec3("Start", from);
	m_debugLineShader.setVec3("End", to);
	m_debugLineShader.setMat4("view", m_renderCamera->getViewMat());
	m_debugLineShader.setMat4("projection", m_renderCamera->getProjectionMat());
	m_debugLineShader.setMat4("model", glm::mat4(1.0f));
	m_debugLineShader.setVec3("color", color);
	glBindVertexArray(lineVAO);
	glDrawArrays(GL_POINTS, 0, 2);
	glBindVertexArray(0);
	unBind();
}

void DebugBuffer::drawCircle(const glm::vec3& center, const float& radius, const glm::vec3& color)
{
}


void DebugBuffer::Draw()
{
	// return to default buffer
	unBind();
	// clear default buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// use screen shader
	screenShader->use();
	// draw
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// using color attachment
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	// setting transform uniforms
	updateTransformUniforms();
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	renderQuad();
}

void DebugBuffer::attachBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	blitDepthBuffer();
}



