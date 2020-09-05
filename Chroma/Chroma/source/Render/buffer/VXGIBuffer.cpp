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
		//// Voxelize the scene
		//Voxelize();

		//// Switch to VXGI buffer
		//Bind();

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
		m_Voxel3DTexture = new Texture3D(m_VoxelGridTextureSize, m_VoxelGridTextureSize, m_VoxelGridTextureSize, true);

		// Set up GL Points for Voxel Visualization
		SetupVoxelVisualizationVAO();

	}

	void VXGIBuffer::SetupVoxelVisualizationVAO()
	{
		std::vector<glm::vec3> voxelVerts(m_NumVoxels);

		// Generate buffers
		// Vertex Array Object Buffer
		glGenVertexArrays(1, &m_VoxelVisualizationVAO);
		// Vertex Buffer and Element Buffer
		unsigned int VBO;
		glGenBuffers(1, &VBO);

		// Bind buffers
		glBindVertexArray(m_VoxelVisualizationVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, m_NumVoxels * sizeof(glm::vec3) , &voxelVerts[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	}

	void VXGIBuffer::DrawVoxelVisualization()
	{
		// -------------------------------------------------------
		// Render 3D texture to screen.
		// -------------------------------------------------------

		Bind();

		// Bind Voxel Visualization Shader
		m_VoxelVisualizationShader.Use();

		// Set voxel shader uniforms
		UpdateVoxelShaderUniforms(m_VoxelVisualizationShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, m_Voxel3DTexture->GetID());
		m_VoxelVisualizationShader.SetUniform("voxelTexture", 0);
		
		// Render Grid VAO
		glBindVertexArray(m_VoxelVisualizationVAO);
		glDrawArrays(GL_POINTS, 0, m_NumVoxels);
		glBindVertexArray(0);

		Draw();
	}

	void VXGIBuffer::UpdateVoxelShaderUniforms(Shader& shader)
	{
		shader.SetUniform("voxelGridResolution", m_Voxel3DTexture->GetTextureData()->depth);
		m_VoxelGridCentroid.x = glm::sin(GAMETIME)* 10.0;
		//m_VoxelGridCentroid.y = glm::sin(GAMETIME) * 10.0;
		//m_VoxelGridCentroid.y = (float)m_VoxelGridTextureSize  * m_VoxelGridSize;
		shader.SetUniform("voxelGridCentroid", m_VoxelGridCentroid);
		shader.SetUniform("voxelGridSize", m_VoxelGridSize);
	}

	void VXGIBuffer::Voxelize()
	{
		// first we clear the previous voxelization
		m_Voxel3DTexture->Clear(glm::vec4(0.0));

		// Use the voxelshader
		m_VoxelShader.Use();

		// Set voxel shader uniforms
		UpdateVoxelShaderUniforms(m_VoxelShader);

		// Set to default framebuffer
		UnBind();

		// Set up for scene render
		glViewport(0,0, m_VoxelGridTextureSize, m_VoxelGridTextureSize);
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

			// Shadowmap
			/*glActiveTexture(GL_TEXTURE0 + meshComponent->GetMaterial().GetNumTextures() + 1);
			m_VoxelShader.SetUniform("shadowmap", meshComponent->GetMaterial().GetNumTextures() + 1);
			glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<ShadowBuffer*>(Chroma::Render::GetShadowBuffer())->GetTexture());*/

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
