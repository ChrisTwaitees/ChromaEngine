#include "DebugBuffer.h"

void DebugBuffer::generateLineVAO()
{
	// Generate buffers
	// Vertex Array Object Buffer
	glGenVertexArrays(1, &debugVAO);
	// Vertex Buffer and Element Buffer
	glGenBuffers(1, &debugVBO);

	// Bind buffers
	glBindVertexArray(debugVAO);
	glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ChromaVertex), &vertices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)0);
	glBindVertexArray(0);

}

void DebugBuffer::drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color)
{

	ChromaVertex fromVert;
	fromVert.setPosition(from);
	ChromaVertex toVert;
	toVert.setPosition(to);

	vertices = std::vector<ChromaVertex>{ fromVert, toVert };
	generateLineVAO();

	Bind();

	m_constantShader.use();
	m_constantShader.setMat4("view", m_renderCamera->getViewMat());
	m_constantShader.setMat4("projection", m_renderCamera->getProjectionMat());
	m_constantShader.setMat4("model", glm::mat4(1.0f));
	m_constantShader.setVec3("lightColor", glm::vec3(1.0f));
	m_constantShader.setFloat("lightIntensity", 1.0f);
	m_boxPrimitive->Draw(m_constantShader);

	m_debugLineShader.use();
	m_debugLineShader.setMat4("view", m_renderCamera->getViewMat());
	m_debugLineShader.setMat4("projection", m_renderCamera->getProjectionMat());
	m_debugLineShader.setMat4("model", glm::mat4(1.0f));
	m_debugLineShader.setVec3("color", color);
	glBindVertexArray(debugVAO);
	glDrawArrays(GL_POINTS, 0, 2);
	glBindVertexArray(0);
	unBind();
}

void DebugBuffer::drawCircle(const glm::vec3& center, const float& radius, const glm::vec3& color)
{
}


void DebugBuffer::Draw()
{
	drawLine(glm::vec3(0.0), glm::vec3(10.5), glm::vec3(0.0, 1.0, 0.0));
	// clear buffers
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
	renderQuad();
}


