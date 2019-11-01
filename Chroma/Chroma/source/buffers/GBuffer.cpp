#include "GBuffer.h"


void GBuffer::initialize()
{
	// create the buffer
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	// - position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// - normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// - color + roughness color buffer
	glGenTextures(1, &gAlbedoRoughness);
	glBindTexture(GL_TEXTURE_2D, gAlbedoRoughness);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoRoughness, 0);

	// - metalness/specular color buffer
	glGenTextures(1, &gMetalnessSpecular);
	glBindTexture(GL_TEXTURE_2D, gMetalnessSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gMetalnessSpecular, 0);

	// - fragposLightSpace color buffer for shadowmapping
	glGenTextures(1, &gFragPosLightSpace);
	glBindTexture(GL_TEXTURE_2D, gFragPosLightSpace);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gFragPosLightSpace, 0);

	// - tell OpenGL which color attachments we'll use for rendering 
	unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, attachments); 
	// create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// configure shaders
	configureShaders();
}

void GBuffer::updateTransformUniforms()
{

	lightingPassShader.use();
	lightingPassShader.setVec2("scale", scale);
	lightingPassShader.setVec2("offset", offset);

}

void GBuffer::configureShaders()
{
	lightingPassShader.use();
	lightingPassShader.setInt("gPosition", 0);
	lightingPassShader.setInt("gNormal", 1);
	lightingPassShader.setInt("gAlbedoRoughness", 2);
	lightingPassShader.setInt("gMetalnessSpecular", 3);
	lightingPassShader.setInt("gFragPosLightSpace", 4);
	lightingPassShader.setInt("gShadowmap", 5);
	lightingPassShader.setInt("SSAO", 6);
}

void GBuffer::bindAllGBufferTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoRoughness);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gMetalnessSpecular);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gFragPosLightSpace);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, mShadowbuffer->ShadowMapTexture.ID);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, ssaoBuffer->getTexture());
}

void GBuffer::setLightingUniforms()
{
	lightingPassShader.setLightingUniforms(mScene->Lights, *mScene->RenderCamera);
	lightingPassShader.setFloat("ambient", 0.5f);
}

void GBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GBuffer::drawGeometryPass()
{
	// 1. geometry pass: render scene's geometry/color data into gbuffer
	//// -----------------------------------------------------------------
	Bind();
	geometryPassShader.use();
	geometryPassShader.setMat4("view", mScene->RenderCamera->viewMat);
	geometryPassShader.setMat4("projection", mScene->RenderCamera->projectionMat);
	geometryPassShader.setMat4("lightSpaceMatrix", mShadowbuffer->getLightSpaceMatrix());
	// Render Scene
	for (ChromaEntity* entity : mScene->Entities)
	{
		glm::mat4 finalTransformMatrix = entity->getTransformationMatrix();
		for (IChromaComponent* component : entity->DefferedComponents)
		{
			geometryPassShader.setMat4("model", finalTransformMatrix);
			component->Draw(geometryPassShader, *mScene->RenderCamera, mScene->Lights, finalTransformMatrix);
		}
	}
	unBind();
}

void GBuffer::drawLightingPass()
{
	glClear(GL_COLOR_BUFFER_BIT);
	// use the lighting pass shader
	lightingPassShader.use();
	// updating transforms
	updateTransformUniforms();
	// activating textures
	bindAllGBufferTextures();
	// set lighting uniforms
	setLightingUniforms();
}


void GBuffer::Draw()
{
	// 1. geometry pass: render scene's geometry/color data into gbuffer
	// -----------------------------------------------------------------
	drawGeometryPass();

	// 1.5 SSAO Pass : draw SSAO to be used during lighting pass
	// -----
	ssaoBuffer->Draw(gPosition, gNormal, mScene->RenderCamera->projectionMat);

	// 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
	// -----------------------------------------------------------------------------------------------------------------------
	drawLightingPass();
	renderQuad();


	// 3. copy content of geometry's depth buffer to default framebuffer's depth buffer
	// ----------------------------------------------------------------------------------
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
	glBlitFramebuffer(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}



GBuffer::GBuffer(const ChromaScene*& Scene, ShadowBuffer*& shadowbuffer)
{
	setupQuad();
	initialize();
	mScene = Scene;
	mShadowbuffer = shadowbuffer;
}

GBuffer::~GBuffer()
{
}