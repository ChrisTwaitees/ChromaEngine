#include "DebugBuffer.h"

void DebugBuffer::initialize()
{
	// build Line VAO
	generatePointVAO();
}

void DebugBuffer::blitDepthBuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_prevFrameBuffer->getFBO()); // fetch depth from postFXBuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); // copy depth to debug buffer
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
}

void DebugBuffer::generatePointVAO()
{
	ChromaVertex singleVert;

	// Generate buffers
	// Vertex Array Object Buffer
	glGenVertexArrays(1, &pointVAO);
	// Vertex Buffer and Element Buffer
	glGenBuffers(1, &pointVBO);

	// Bind buffers
	glBindVertexArray(pointVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
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

void DebugBuffer::drawShapes()
{
	attachBuffer();

	// lines
	for (LineShape line : m_lines)
		renderLine(line);
	// spheres
	for (SphereShape sphere : m_spheres)
		renderSphere(sphere);
	// boxes
	for (BoxShape box : m_boxes)
		renderBox(box);

	unBind();
}

void DebugBuffer::renderLine(LineShape line)
{
	m_LineShader.use();
	m_LineShader.setVec3("Start", line.start);
	m_LineShader.setVec3("End", line.end);
	m_LineShader.setMat4("view", m_renderCamera->getViewMat());
	m_LineShader.setMat4("projection", m_renderCamera->getProjectionMat());
	m_LineShader.setMat4("model", glm::mat4(1.0f));
	m_LineShader.setVec3("color", line.color);
	glBindVertexArray(pointVAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void DebugBuffer::renderSphere(SphereShape sphere)
{
	m_SphereShader.use();
	m_SphereShader.setMat4("VPMat", m_renderCamera->getProjectionMat() * m_renderCamera->getViewMat());
	m_SphereShader.setMat4("model", sphere.transform);
	m_SphereShader.setFloat("radius", sphere.radius);
	m_SphereShader.setVec3("color", sphere.color);
	glBindVertexArray(pointVAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void DebugBuffer::renderBox(BoxShape box)
{
	m_BoxShader.use();
	m_BoxShader.setVec3("BBoxMin", box.bbox_min);
	m_BoxShader.setVec3("BBoxMax", box.bbox_max);
	m_BoxShader.setMat4("VPMat" , m_renderCamera->getProjectionMat() * m_renderCamera->getViewMat());
	m_BoxShader.setMat4("model", box.transform);
	m_BoxShader.setVec3("color", box.color);
	glBindVertexArray(pointVAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void DebugBuffer::drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color)
{
	LineShape new_line;
	new_line.start = from;
	new_line.end = to;
	new_line.color = color;
	m_lines.push_back(new_line);
}

void DebugBuffer::drawBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::vec3& color)
{
	BoxShape new_box;
	new_box.bbox_min = bbMin;
	new_box.bbox_max = bbMax;
	new_box.color = color;
	m_boxes.push_back(new_box);
}

void DebugBuffer::drawBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::mat4& trans, const glm::vec3& color)
{
	BoxShape new_box;
	new_box.bbox_min = bbMin;
	new_box.bbox_max = bbMax;
	new_box.color = color;
	new_box.transform =  trans;
	m_boxes.push_back(new_box);
}

void DebugBuffer::drawSphere(const glm::vec3& center, const float& radius, const glm::vec3& color)
{
	SphereShape new_sphere;
	new_sphere.transform = glm::translate(new_sphere.transform, center);
	new_sphere.radius = radius;
	new_sphere.color = color;
	m_spheres.push_back(new_sphere);
}


void DebugBuffer::ClearBuffer()
{
	Bind();
	m_lines.clear();
	m_spheres.clear();
	m_boxes.clear();
	unBind();
}

void DebugBuffer::Draw()
{
	drawShapes();
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



