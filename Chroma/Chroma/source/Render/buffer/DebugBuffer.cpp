#include "DebugBuffer.h"
#include <scene/ChromaScene.h>
#include <model/Model.h>

void DebugBuffer::Initialize()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// create floating point color buffer
	glGenTextures(1, &FBOTexture);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOTexture, 0);
	// create depth buffer (renderbuffer)
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	// attach buffers
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// build Line VAO
	generatePointVAO();
}

void DebugBuffer::blitPostFXBuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_postFXBuffer->getFBO()); // fetch  postFXBuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); // copy depth and color to current buffer
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
}

glm::vec3 DebugBuffer::GetTranslation(glm::mat4 mat4)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat4, scale, rotation, translation, skew, perspective);

	return translation;

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
	m_LineShader.SetMat4("view", m_renderCamera->GetViewMatrix());
	m_LineShader.SetMat4("projection", m_renderCamera->GetProjectionMatrix());
	m_LineShader.SetMat4("model", glm::mat4(1.0f));
	m_LineShader.setVec3("color", line.color);
	glBindVertexArray(pointVAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void DebugBuffer::renderSphere(SphereShape sphere)
{
	m_SphereShader.use();
	m_SphereShader.SetMat4("VPMat", m_renderCamera->GetProjectionMatrix() * m_renderCamera->GetViewMatrix());
	m_SphereShader.SetMat4("model", sphere.transform);
	m_SphereShader.SetFloat("radius", sphere.radius);
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
	m_BoxShader.SetMat4("VPMat" , m_renderCamera->GetProjectionMatrix() * m_renderCamera->GetViewMatrix());
	m_BoxShader.SetMat4("model", box.transform);
	m_BoxShader.setVec3("color", box.color);
	glBindVertexArray(pointVAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}


void DebugBuffer::DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color)
{
	LineShape new_line;
	new_line.start = from;
	new_line.end = to;
	new_line.color = color;
	m_lines.push_back(new_line);
}

void DebugBuffer::DrawBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::vec3& color)
{
	BoxShape new_box;
	new_box.bbox_min = bbMin;
	new_box.bbox_max = bbMax;
	new_box.color = color;
	m_boxes.push_back(new_box);
}

void DebugBuffer::DrawBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::mat4& trans, const glm::vec3& color)
{
	BoxShape new_box;
	new_box.bbox_min = bbMin;
	new_box.bbox_max = bbMax;
	new_box.color = color;
	new_box.transform =  trans;
	m_boxes.push_back(new_box);
}

void DebugBuffer::DrawSphere(const glm::vec3& center, const float& radius, const glm::vec3& color)
{
	SphereShape new_sphere;
	new_sphere.transform = glm::translate(new_sphere.transform, center);
	new_sphere.radius = radius;
	new_sphere.color = color;
	m_spheres.push_back(new_sphere);
}

void DebugBuffer::DrawSceneSkeletons(ChromaScene* const& scene)
{
	for (IChromaEntity* entity : scene->getEntities())
	{
		for (IChromaComponent* component : entity->getMeshComponents())
		{
			// check if mesh skinned
			if (((ChromaMeshComponent*)component)->m_IsSkinned)
			{
				((Model*)component)->GetSkeleton()->DebugDraw(this);
			}
		}
	}
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
	// 1. Draw shapes to debugbuffer after fetching postFX color and depth
	drawShapes();

	// 2. Bind postFX buffer to draw to
	m_postFXBuffer->Bind();

	// use screen shader
	screenShader->use();
	// draw
	// using color attachment
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOTexture);
	// setting transform uniforms
	updateTransformUniforms();
	renderQuad();
	// return to default frambuffer
	m_postFXBuffer->unBind();
}

void DebugBuffer::attachBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	blitPostFXBuffer();
}



