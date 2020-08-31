#include "VXGIBuffer.h"
#include "scene/Scene.h"
#include "core/Application.h"
#include "render/Render.h"
#include "shadow/ShadowBuffer.h"


namespace Chroma
{
	VXGIBuffer::~VXGIBuffer()
	{
		if (m_Voxel3DTexture) delete m_Voxel3DTexture;
		if (m_ScreenShader) delete m_ScreenShader;
	}

	void VXGIBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void VXGIBuffer::Draw()
	{
		// Voxelize the scene
		Voxelize();

		// Switch to VXGI buffer
		Bind();

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
		else
		{
			//Bind();
			// Draw the Framebuffer
			//m_ScreenShader->Use();
			//m_ScreenShader->SetUniform("screenTexture", 0);
			//// using color attachment
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
			//// setting transform uniforms
			//SetTransformUniforms();
			//RenderQuad();

		}

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
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			CHROMA_WARN("Framebuffer not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Create 3D Voxel Texture
		m_Voxel3DTexture = new Texture3D(m_VoxelTextureSize, m_VoxelTextureSize, m_VoxelTextureSize, true);

	}

	void VXGIBuffer::DrawVoxelVisualization()
	{
		// Render with WS Position Shader
		m_VoxelWorldPositionShader.Use();
		m_VoxelWorldPositionShader.SetUniform("model", glm::mat4(1.0));

		// Settings.
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		// Back.
		glCullFace(GL_FRONT);
		m_VVFBO1.Bind();
		m_Cube.BindDrawVAO();

		// Front.
		glCullFace(GL_BACK);
		m_VVFBO2.Bind();
		m_Cube.BindDrawVAO();

		// -------------------------------------------------------
		// Render 3D texture to screen.
		// -------------------------------------------------------
		m_VoxelVisualizationShader.Use();

		// Set to VXGI Buffer
		Bind();

		// Settings.
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// Activate textures.
		// Back
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,  m_VVFBO1.GetTexture());
		m_VoxelVisualizationShader.SetUniform("textureBack", 0);
		// Front
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_VVFBO2.GetTexture());
		m_VoxelVisualizationShader.SetUniform("textureBack", 1);
		// Voxel
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_3D, m_Voxel3DTexture->GetID());
		m_VoxelVisualizationShader.SetUniform("texture3D", 2);

		// Render
		m_VoxelVisualizationShader.SetUniform("scale", m_Scale);
		m_VoxelVisualizationShader.SetUniform("offset", m_Offset);
		RenderQuad();


	}

	void VXGIBuffer::Voxelize()
	{
		// first we clear the previous voxelization
		m_Voxel3DTexture->Clear(glm::vec4(0.0));

		// Use the voxelshader
		m_VoxelShader.Use();

		// Set voxel shader uniforms
		m_VoxelShader.SetUniform("voxelResolution", m_Voxel3DTexture->GetTextureData()->depth);
		m_VoxelShader.SetUniform("voxelGridCentroid", glm::vec3(0.0, 0.0, 0.0));

		// Set to default framebuffer
		UnBind();

		// Set up for scene render
		glViewport(0,0, m_VoxelTextureSize, m_VoxelTextureSize);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		// Texture
		glActiveTexture(GL_TEXTURE0);
		m_Voxel3DTexture->Bind();
		m_VoxelShader.SetUniform("texture3D", 0);
		glBindImageTexture(0, m_Voxel3DTexture->GetID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

		// Render Scene
		m_VoxelShader.SetUniform("lightSpaceMatrix", static_cast<ShadowBuffer*>(Render::GetShadowBuffer())->GetLightSpaceMatrix());
		for (const UID& uid : Scene::GetLitComponentUIDs())
		{
			// SET UNIFORMS AND DRAW
			MeshComponent* meshComponent = static_cast<MeshComponent*>(Scene::GetComponent(uid));

			// model transform
			m_VoxelShader.SetUniform("model", meshComponent->GetWorldTransform());

			// Irradiance
			glActiveTexture(GL_TEXTURE0 + meshComponent->GetMaterial().GetNumTextures() + 1);
			m_VoxelShader.SetUniform("irradianceMap", meshComponent->GetMaterial().GetNumTextures());
			glBindTexture(GL_TEXTURE_CUBE_MAP, Scene::GetIBL()->GetIrradianceMapID());
			// Prefilter Map
			glActiveTexture(GL_TEXTURE0 + meshComponent->GetMaterial().GetNumTextures() + 2);
			m_VoxelShader.SetUniform("prefilterMap", meshComponent->GetMaterial().GetNumTextures() + 2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, Chroma::Scene::GetIBL()->GetPrefilterMapID());
			// BRDF LUT
			glActiveTexture(GL_TEXTURE0 + meshComponent->GetMaterial().GetNumTextures() + 3);
			m_VoxelShader.SetUniform("brdfLUT", meshComponent->GetMaterial().GetNumTextures() + 3);
			glBindTexture(GL_TEXTURE_2D, Chroma::Scene::GetIBL()->GetBRDFLUTID());
			glActiveTexture(GL_TEXTURE0 + meshComponent->GetMaterial().GetNumTextures() + 4);
			m_VoxelShader.SetUniform("shadowmap", meshComponent->GetMaterial().GetNumTextures() + 4);
			glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<ShadowBuffer*>(Chroma::Render::GetShadowBuffer())->GetTexture());

			meshComponent->DrawUpdateMaterials(m_VoxelShader);
		}

		// Regenerate Voxel3DTexture MipMaps
		glGenerateMipmap(GL_TEXTURE_3D);

		// Color Mask
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		// Reset Viewport
		glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	}
}
