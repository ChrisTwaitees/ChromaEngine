#include "VXGIBuffer.h"
#include "scene/Scene.h"
#include "core/Application.h"
#include "render/Render.h"
#include "shadow/ShadowBuffer.h"
#include "buffer/DebugBuffer.h"
#include "editor/ui/EditorUI.h"


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
		//m_ScreenShader->Use();
		//m_ScreenShader->SetUniform("screenTexture", 0);
		//// using color attachment
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
		//// setting transform uniforms
		//SetTransformUniforms();
		//RenderQuad();
	}

	void VXGIBuffer::Draw(const bool& visualizeVoxelization)
	{

		if (EditorUI::m_VXGI && m_VoxelGridMovedThisFrame)
		{
			Voxelize();
		}


		// Visualization
		if (visualizeVoxelization)
		{
			DrawVoxelVisualization();
		}
		else
		{
			Bind();

			ConeTraceScene();

			RenderQuad();
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);

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
		// Generate FBO texture
		glGenTextures(1, &m_FBOTexture);
		glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);

		// Texture parms
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

		// Update Voxel Grid 
		UpdateVoxelGridSize();
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

		// clear vector
		voxelVerts.clear();
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

		// Bind voxel grid
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, m_Voxel3DTexture->GetID());
		m_VoxelVisualizationShader.SetUniform("voxelTexture", 0);
		
		// Render Grid VAO
		glBindVertexArray(m_VoxelVisualizationVAO);
		glDrawArrays(GL_POINTS, 0, m_NumVoxels);
		glBindVertexArray(0);

		Draw();
	}

	void VXGIBuffer::ConeTraceScene()
	{
		// Draw Screen Quad
		m_VoxelConeTracing.Use();

		// Set ScreenQuad transforms
		m_VoxelConeTracing.SetUniform("scale", m_Scale);
		m_VoxelConeTracing.SetUniform("offset", m_Offset);

		// Set uniforms
		// Grid Uniforms
		UpdateVoxelShaderUniforms(m_VoxelConeTracing);
		// Voxel Cone Tracing Uniforms
		m_VoxelConeTracing.SetUniform("u_VoxelRayMaxDistance", 20.0f);
		m_VoxelConeTracing.SetUniform("u_VoxelRayStepSize", 0.75f);
		m_VoxelConeTracing.SetUniform("u_VoxelNumCones", 16);

		// Bind WS Positions and Normals and MetRoughAO
		m_VoxelConeTracing.SetUniform("u_PositionsWS", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Render::GetWSPositions());

		m_VoxelConeTracing.SetUniform("u_NormalsWS", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Render::GetWSNormals());

		m_VoxelConeTracing.SetUniform("u_MetRoughAO", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, Render::GetMetRoughAO());

		// Bind voxel grid
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_3D, m_Voxel3DTexture->GetID());
		m_VoxelConeTracing.SetUniform("u_VoxelTexture", 3);
	}

	void VXGIBuffer::UpdateVoxelShaderUniforms(Shader& shader)
	{
		shader.SetUniform("u_VoxelGridResolution", m_Voxel3DTexture->GetTextureData()->depth);
		shader.SetUniform("u_VoxelGridSize", m_VoxelGridSize);
		shader.SetUniform("u_VoxelGridCentroid", m_VoxelGridCentroid);
	}

	std::pair<glm::vec3, glm::vec3> VXGIBuffer::GetVoxelGridHalfExtents()
	{
		glm::vec3 extents((float)m_VoxelGridTextureSize / (1.0f / m_VoxelGridSize));
		return  std::make_pair<glm::vec3, glm::vec3>(m_VoxelGridCentroid - extents, m_VoxelGridCentroid + extents);
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
		// Nvidia Extension to try and extend rasterization borders and capture thin
		// meshes at grazing angles
		if (GL_CONSERVATIVE_RASTERIZATION_NV)
		{
			CHROMA_INFO("Conservative Rasterization Supported!");
			glEnable(GL_CONSERVATIVE_RASTERIZATION_NV);
			glSubpixelPrecisionBiasNV(5,5);
		}


		// Texture
		glActiveTexture(GL_TEXTURE0);
		m_Voxel3DTexture->Bind();
		m_VoxelShader.SetUniform("u_VoxelTexture", 0);
		glBindImageTexture(0, m_Voxel3DTexture->GetID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);

		// Render Scene
		m_VoxelShader.SetUniform("lightSpaceMatrix", static_cast<ShadowBuffer*>(Render::GetShadowBuffer())->GetLightSpaceMatrix());
		for (const UID& uid : Scene::GetLitComponentUIDs())
		{
			// SET UNIFORMS AND DRAW
			MeshComponent* meshComponent = static_cast<MeshComponent*>(Scene::GetComponent(uid));

			// model transform
			m_VoxelShader.SetUniform("model", meshComponent->GetWorldTransform());

			// Shadowmap
			glActiveTexture(GL_TEXTURE0 + meshComponent->GetMaterial().GetNumTextures() + 1);
			m_VoxelShader.SetUniform("shadowmap", meshComponent->GetMaterial().GetNumTextures() + 1);
			glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<ShadowBuffer*>(Chroma::Render::GetShadowBuffer())->GetTexture());

			meshComponent->DrawUpdateMaterials(m_VoxelShader);
		}

		//  !!! SLOW !!!
		// Regenerate Voxel3DTexture MipMaps
		glGenerateMipmap(GL_TEXTURE_3D);

		// Reset RenderState
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		if (GL_CONSERVATIVE_RASTERIZATION_NV)
		{
			glDisable(GL_CONSERVATIVE_RASTERIZATION_NV);
		}

		// Reset Viewport
		glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());


	}

	bool VXGIBuffer::OnCameraMoved(CameraMovedEvent& e)
	{
		// Calculate Voxel Grid Centroid and Size based on Camera Frustum
		// Collect Shadow Cascade BBox Data
		for (const float& distance : static_cast<ShadowBuffer*>(Render::GetShadowBuffer())->m_CascadeSplitDistances)
			CHROMA_INFO("Distance : {0}", distance);

		Camera* renderCam = Scene::GetRenderCamera();
		glm::vec3 startPos = renderCam->GetPosition() + (renderCam->GetDirection() * renderCam->GetNearDist());
		glm::vec3 endPos = renderCam->GetPosition() + (renderCam->GetDirection() * static_cast<ShadowBuffer*>(Render::GetShadowBuffer())->m_CascadeSplitDistances[0]);

		// Calculate camera frustum center
		//glm::vec3 nearGrid = renderCam->GetPosition() + (renderCam->GetDirection() * glm::vec3(renderCam->GetNearDist() * 3.0));
		glm::vec3 farGrid = renderCam->GetPosition() + (renderCam->GetDirection() * glm::vec3(m_VoxelGridWSSize*2.0f));

		// Update Grid Centroid
		m_VoxelGridMovedThisFrame = glm::distance2(farGrid, m_VoxelGridCentroid) > 0.0f;

		m_VoxelGridCentroid = farGrid;

		//CHROMA_INFO(e.GetPosition().x);
		return true;
	}
}
