#include "DebugBuffer.h"
#include <scene/Scene.h>
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
		CHROMA_WARN("Framebuffer Not Complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// build Line VAO
	generatePointVAO();
}

void DebugBuffer::blitPostFXBuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_PostFXBuffer->getFBO()); // fetch  postFXBuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); // copy depth and color to current buffer
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST
	);

}

void DebugBuffer::blitDepthPostFXBuffer()
{
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

void DebugBuffer::drawShapes()
{
	// OVERLAY 
	AttachBuffer();
	DrawOverlayShapes();

	// DEPTH RESPECTING
	blitDepthPostFXBuffer();
	DrawDepthCulledShapes();

	unBind();
}

void DebugBuffer::DrawOverlayShapes()
{
	// lines
	for (LineShape line : m_OverlayLines)
		renderLine(line);
	// spheres
	for (SphereShape sphere : m_OverlaySpheres)
		renderSphere(sphere);
	// boxes
	for (BoxShape box : m_OverlayBoxes)
		renderBox(box);
	// joints
	for (JointShape joint : m_OverlayJoints)
		renderJoint(joint);
	// coordinates
	for (CoordinatesShape coordinate : m_OverlayCoordinates)
		renderCoordinate(coordinate);
}

void DebugBuffer::DrawDepthCulledShapes()
{
	// lines
	for (LineShape line : m_lines)
		renderLine(line);
	// spheres
	for (SphereShape sphere : m_spheres)
		renderSphere(sphere);
	// boxes
	for (BoxShape box : m_boxes)
		renderBox(box);
	// coordinates
	for (CoordinatesShape coordinate : m_Coordinates)
		renderCoordinate(coordinate);
}

void DebugBuffer::renderLine(LineShape line)
{
	m_LineShader.use();
	m_LineShader.setUniform("Start", line.start);
	m_LineShader.setUniform("End", line.end);
	m_LineShader.setUniform("view", m_RenderCamera->GetViewMatrix());
	m_LineShader.setUniform("projection", m_RenderCamera->GetProjectionMatrix());
	m_LineShader.setUniform("model", glm::mat4(1.0f));
	m_LineShader.setUniform("color", line.color);

	BindPointVAO();
}

void DebugBuffer::renderSphere(SphereShape sphere)
{
	m_SphereShader.use();
	m_SphereShader.setUniform("VPMat", m_RenderCamera->GetProjectionMatrix() * m_RenderCamera->GetViewMatrix());
	m_SphereShader.setUniform("model", sphere.transform);
	m_SphereShader.setUniform("radius", sphere.radius);
	m_SphereShader.setUniform("color", sphere.color);

	BindPointVAO();
}

void DebugBuffer::renderBox(BoxShape box)
{
	m_BoxShader.use();
	m_BoxShader.setUniform("BBoxMin", box.bbox_min);
	m_BoxShader.setUniform("BBoxMax", box.bbox_max);
	m_BoxShader.setUniform("VPMat" , m_RenderCamera->GetProjectionMatrix() * m_RenderCamera->GetViewMatrix());
	m_BoxShader.setUniform("model", box.transform);
	m_BoxShader.setUniform("color", box.color);

	BindPointVAO();
}

void DebugBuffer::renderJoint(JointShape joint)
{
	m_JointShader.use();
	m_JointShader.setUniform("JointPos", joint.jointPos);
	m_JointShader.setUniform("ChildPos", joint.childPos);
	m_JointShader.setUniform("Size", joint.size);
	m_JointShader.setUniform("VPMat", m_RenderCamera->GetProjectionMatrix() * m_RenderCamera->GetViewMatrix());
	m_JointShader.setUniform("transform", joint.transform);
	m_JointShader.setUniform("color", joint.color);
	BindPointVAO();
}

void DebugBuffer::renderCoordinate(CoordinatesShape coordinate)
{
	// Coordinate reference 
	m_CoordinatesShader.use();
	m_CoordinatesShader.setUniform("VPMat", m_RenderCamera->GetProjectionMatrix() * m_RenderCamera->GetViewMatrix());
	m_CoordinatesShader.setUniform("transform", coordinate.transform);
	m_CoordinatesShader.setUniform("Size", coordinate.size);

	BindPointVAO();
}

void DebugBuffer::BindPointVAO()
{
	glBindVertexArray(pointVAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}


void DebugBuffer::DrawCoordinates(const glm::mat4& transform, const float& size)
{
	CoordinatesShape newCoordinate;
	newCoordinate.transform = transform;
	newCoordinate.size = size;
	m_Coordinates.push_back(newCoordinate);
}

void DebugBuffer::DrawOverlayCoordinates(const glm::mat4& transform, const float& size)
{
	CoordinatesShape newCoordinate;
	newCoordinate.transform = transform;
	newCoordinate.size = size;
	m_OverlayCoordinates.push_back(newCoordinate);
}

void DebugBuffer::DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color)
{
	LineShape new_line;
	new_line.start = from;
	new_line.end = to;
	new_line.color = color;
	m_lines.push_back(new_line);
}

void DebugBuffer::DrawOverlayLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color)
{
	LineShape new_line;
	new_line.start = from;
	new_line.end = to;
	new_line.color = color;
	m_OverlayLines.push_back(new_line);
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

void DebugBuffer::DrawOverlayBox(const glm::vec3& bbMin, const glm::vec3& bbMax, const glm::vec3& color)
{
	BoxShape new_box;
	new_box.bbox_min = bbMin;
	new_box.bbox_max = bbMax;
	new_box.color = color;
	m_OverlayBoxes.push_back(new_box);
}

void DebugBuffer::DrawSphere(const glm::vec3& center, const float& radius, const glm::vec3& color)
{
	SphereShape new_sphere;
	new_sphere.transform = glm::translate(new_sphere.transform, center);
	new_sphere.radius = radius;
	new_sphere.color = color;
	m_spheres.push_back(new_sphere);
}

void DebugBuffer::DrawOverlaySphere(const glm::vec3& center, const float& radius, const glm::vec3& color)
{
	SphereShape new_sphere;
	new_sphere.transform = glm::translate(new_sphere.transform, center);
	new_sphere.radius = radius;
	new_sphere.color = color;
	m_OverlaySpheres.push_back(new_sphere);
}

void DebugBuffer::DrawOverlayJoint(const glm::vec3& originPosition, const glm::vec3 childPosition, const glm::mat4 jointTransform, const float& size, const glm::vec3& color)
{
	JointShape newJoint;
	newJoint.transform = jointTransform;
	newJoint.jointPos = originPosition;
	newJoint.childPos = childPosition;
	newJoint.size = size;
	newJoint.color = color;
	m_OverlayJoints.push_back(newJoint);
}

void DebugBuffer::DrawSceneSkeletons(Scene* const& m_Scene)
{
	for (std::string const& UID : m_Scene->GetAnimatedEntityUIDs())
	{
		for (IComponent* component : m_Scene->GetEntity(UID)->getMeshComponents())
		{
			// check if mesh skinned
			if (((MeshComponent*)component)->m_IsSkinned)
			{
				((Model*)component)->GetSkeleton()->DebugDraw(this);
			}
		}
	}
}


void DebugBuffer::ClearBuffer()
{
	Bind();

	m_OverlayLines.clear();
	m_OverlayBoxes.clear();
	m_OverlaySpheres.clear();
	m_OverlayJoints.clear();
	m_OverlayCoordinates.clear();

	m_lines.clear();
	m_spheres.clear();
	m_boxes.clear();
	m_Coordinates.clear();

	unBind();
}

void DebugBuffer::Draw()
{
	// 1. Draw shapes to debugbuffer after fetching postFX color and depth
	drawShapes();

	// 2. Bind postFX buffer to draw to
	m_PostFXBuffer->Bind();

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
	m_PostFXBuffer->unBind();
}

void DebugBuffer::AttachBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	blitPostFXBuffer();
}




