#include "VXGIBuffer.h"
#include "scene/Scene.h"
#include "core/Application.h"


namespace Chroma
{
	void VXGIBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void VXGIBuffer::Draw()
	{
		// Voxelize the scene
		Voxelize();

		// Draw the Framebuffer
		m_ScreenShader->Use();
		m_ScreenShader->SetUniform("screenTexture", 0);
		// using color attachment
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
		// setting transform uniforms
		SetTransformUniforms();
		RenderQuad();
	}

	void VXGIBuffer::Draw(const bool& visualizeVoxelization)
	{
		// Voxelize the scene
		Voxelize();

		// Visualization
		if (visualizeVoxelization)
		{
			DrawVoxelVisualization();
		}

		// Draw the Framebuffer
		m_ScreenShader->Use();
		m_ScreenShader->SetUniform("screenTexture", 0);
		// using color attachment
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
		// setting transform uniforms
		SetTransformUniforms();
		RenderQuad();
	}


	void VXGIBuffer::GenTexture()
	{
		// Generate FBO texture
		glGenTextures(1, &m_FBOTexture);
		glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		// Texture parms
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void VXGIBuffer::ResizeBuffers()
	{
		// textures
		glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		// rbo
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		// attach buffers
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);

		// Resize the VVFBOS (Voxel Visualization FBOs)
		m_VVFBO1.ScreenResizeCallback(m_Width, m_Height);
		m_VVFBO2.ScreenResizeCallback(m_Width, m_Height);

	}

	void VXGIBuffer::Init()
	{
		// Initialize Framebuffers
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		// Create and attach Framebuffer texture
		GenTexture();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTexture, 0);

		// create depth buffer (renderbuffer)
		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		// msaa 
		// attach buffers
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
		//glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_SAMPLES, GL_DEPTH_COMPONENT, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			CHROMA_WARN("Framebuffer not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Create 3D Voxel Texture
		m_Voxel3DTexture = std::make_unique<Texture3D>(m_VoxelTextureSize, m_VoxelTextureSize, m_VoxelTextureSize, true);

		// Voxel Visualization
		m_Box = std::make_unique<BoxPrimitive>(BoxPrimitive());
	}

	void VXGIBuffer::DrawVoxelVisualization()
	{
	}

	void VXGIBuffer::Voxelize()
	{
		// Switch to VXGI buffer
		Bind();

		// first we clear the previous voxelization
		m_Voxel3DTexture->Clear(glm::vec4(0.0));

		//// Use the voxelshader
		m_VoxelShader.Use();

		// Set up for scene render
		glViewport(0,0, m_VoxelTextureSize, m_VoxelTextureSize);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		//// Texture
		glActiveTexture(GL_TEXTURE0);
		m_Voxel3DTexture->Bind();
		m_VoxelShader.SetUniform("texture3D", 0);

		// Render Scene
		for (const UID& uid : Scene::GetLitComponentUIDs())
		{
			// transform components by entity transform
			m_VoxelShader.SetUniform("model", static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->GetWorldTransform());
			static_cast<MeshComponent*>(Scene::GetComponent(uid))->DrawUpdateMaterials(m_VoxelShader);
		}

		// Regenerate Voxel3DTexture MipMaps
		glGenerateMipmap(GL_TEXTURE_3D);

		// Color Mask
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		// Reset Buffer and Viewport
		UnBind();
		glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	}
}
