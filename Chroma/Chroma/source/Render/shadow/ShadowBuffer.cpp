#include "ShadowBuffer.h"
#include <component/MeshComponent.h>
#include <scene/Scene.h>
#include <input/Input.h>
#include <core/Application.h>
#include <editor/ui/EditorUI.h>

namespace Chroma
{

	void ShadowBuffer::BuildCSMTextureArray()
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_CascadedTexureArray);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, m_ShadowMapSize, m_ShadowMapSize, SHADOW_NUMCASCADES, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	void ShadowBuffer::CalculateCascadeLightSpaceMatrices()
	{
		// 1. Calculate cascade split distances
		CalculateCascadeSplitDistances();

		// 2. Calculate the light's view projection of each split plane
		m_CascadeLightSpaceMatrices.clear();
		for (unsigned i = 0; i < m_CascadeSplitDistances.size(); i++)
		{
			// First we transform the Normalized Device Coordinates of a unit 1 Frustrum into WS using the 
			// invViewProjection of our camera
			glm::vec3 frustumCornersWS[8] =
			{
				glm::vec3(-1.0f, 1.0f, -1.0f),
				glm::vec3(1.0f, 1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f, 1.0f, 1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(1.0f, -1.0f, 1.0f),
				glm::vec3(-1.0f, -1.0f, 1.0f),
			};
			// transform NDC of frustrum to WS

			glm::mat4 invViewProj = glm::inverse( Chroma::Scene::GetRenderCamera()->GetViewProjMatrix());
			for (unsigned int j = 0; j < 8; ++j)
			{
				glm::vec4 inversePoint = invViewProj * glm::vec4(frustumCornersWS[j], 1.0f);
				frustumCornersWS[j] = glm::vec3(inversePoint / inversePoint.w);
			}

			// We'll create a ray between the corresponding near and corresponding far corner, normalize
			// then multiply by our current partition's distance to our next partition
			glm::vec3 farCornerRay = glm::vec3(0.0);
			glm::vec3 nearCornerRay = glm::vec3(0.0);

			for (unsigned int j = 0; j < 4; ++j)
			{
				glm::vec3 cornerRay = glm::normalize(frustumCornersWS[j + 4] - frustumCornersWS[j]);
				if (i == 0) { // if this is the firs split the near corner is the camera near
					farCornerRay = cornerRay * m_CascadeSplitDistances[i];
					nearCornerRay = cornerRay * Chroma::Scene::GetRenderCamera()->GetNearDist();
				}
				else{
					farCornerRay = cornerRay * m_CascadeSplitDistances[i];
					nearCornerRay = cornerRay * m_CascadeSplitDistances[i-1];
				}
				frustumCornersWS[j + 4] = frustumCornersWS[j] + farCornerRay;
				frustumCornersWS[j] = frustumCornersWS[j] + nearCornerRay;
			}

			// Then we get the longest radius of this slice and use it as the basis for our AABB.
			glm::vec3 frustumCenter = glm::vec3(0.0f);
			for (unsigned int j = 0; j < 8; ++j)
				frustumCenter += frustumCornersWS[j];

			frustumCenter /= 8.0f;

			float shadowFar = -INFINITY;
			float shadowNear = INFINITY;

			float radius = 0.0f;
			for (unsigned int j = 0; j < 8; ++j)
			{
				float distance = glm::length(frustumCornersWS[j] - frustumCenter);
				radius = glm::max(radius, distance);
			}
			radius = std::ceil(radius * 16.0f) / 16.0f;

			glm::vec3 maxExtents = glm::vec3(radius, radius, radius);
			glm::vec3 minExtents = -maxExtents;

			//Position the viewmatrix looking down the center of the frustum using the light direction
			glm::vec3 lightDirection = frustumCenter - glm::normalize(Chroma::Scene::GetSunLight()->GetDirection()) * -minExtents.z;
			m_LightSpaceMatrix = glm::lookAt(lightDirection, frustumCenter, CHROMA_UP);

			glm::vec3 cascadeExtents = maxExtents - minExtents;

			glm::mat4 m_LightOrthoMatrix = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0.0f, cascadeExtents.z);

			//In order to avoid light shimmering we need to create a rounding matrix so we move in texel sized increments.
			//You can think of it as finding out how much we need to move the orthographic matrix so it matches up with shadow map, it is done like this:
			glm::mat4 shadowMatrix = m_LightOrthoMatrix * m_LightSpaceMatrix;
			glm::vec4 shadowOrigin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			shadowOrigin = shadowMatrix * shadowOrigin;
			shadowOrigin = shadowOrigin * (float)(m_ShadowMapSize) / 2.0f;

			glm::vec4 roundedOrigin = glm::round(shadowOrigin);
			glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
			roundOffset = roundOffset * 2.0f / (float)(m_ShadowMapSize);
			roundOffset.z = 0.0f;
			roundOffset.w = 0.0f;

			glm::mat4 shadowProj = m_LightOrthoMatrix;
			shadowProj[3] += roundOffset;
			m_CascadeLightSpaceMatrices.push_back(shadowProj * m_LightSpaceMatrix);
		}
	}

	void ShadowBuffer::CalculateCascadeSplitDistances()
	{
		// 1.1 Calculate the distances of the Cascade Splits
		// Using the practical split scheme we'll shorten the distances to the split planes near to the camera
		// as proposed here : https://developer.nvidia.com/gpugems/gpugems3/part-ii-light-and-shadows/chapter-10-parallel-split-shadow-maps-programmable-gpus
		// this proposes the distances based on logirithmic and linear distances

		// Empty array
		m_CascadeSplitDistances.clear();

		// Get the camera near and far
		float camNear = Chroma::Scene::GetRenderCamera()->GetNearDist();
		float camFar = Chroma::Scene::GetRenderCamera()->GetFarDist();

		// Calculate the Cascade Split distances
		for (unsigned int i = 0; i < SHADOW_NUMCASCADES; ++i)
		{
			float step = static_cast<float>(i + 1) / static_cast<float>(SHADOW_NUMCASCADES);
			float log = camNear * glm::pow((camFar / camNear), step);
			float uniform = camNear + (camFar - camNear) * step;
			m_CascadeSplitDistances.push_back((m_CascadeSplitDistanceRatio * log) + ((1.0 - m_CascadeSplitDistanceRatio) * uniform));
		}
	}

	void ShadowBuffer::Init()
	{
		// Create frame buffer to store depth to
		glGenFramebuffers(1, &m_CascadeShadowFBO);

		// Generate cascade textures
		glGenTextures(1, &m_CascadedTexureArray);
		BuildCSMTextureArray();

		// Set texture array as depth attachment
		glBindFramebuffer(GL_FRAMEBUFFER, m_CascadeShadowFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_CascadedTexureArray, 0);
	
		// Setting Color to NONE
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		// Set back to default FBO
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Calculate split matrices
		CalculateCascadeLightSpaceMatrices();
	}


	void ShadowBuffer::DrawShadowMaps()
	{
		CHROMA_PROFILE_FUNCTION();
	
		// Bind Shadow Depth Framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_CascadeShadowFBO);
		// Set Viewport to Texture dimensions
		glViewport(0, 0, m_ShadowMapSize, m_ShadowMapSize);

		// Set gl depth settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_CLAMP);
		glCullFace(GL_FRONT);
		// Iterate over each cascade frustrum
		for (unsigned int i = 0; i < SHADOW_NUMCASCADES; i++)
		{
			// Set to current texture in array
			glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_CascadedTexureArray, 0, i);
			// Clear previous frame's depth 
			glClear(GL_DEPTH_BUFFER_BIT);

			// Set up depth shader
			m_DepthShader.Use();
			//if (i == 0)
			//{
			//	//calculate LightSpaceMatrix
			//	float near_plane = 0.01f, far_plane = 50.0f;
			//	glm::mat4 lightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, near_plane, far_plane);
			//	glm::mat4 lightView = glm::lookAt(Chroma::Scene::GetSunLight()->GetDirection() * -15.0f,
			//		glm::vec3(0.0),
			//		glm::vec3(0.0f, 1.0f, 0.0f));
			//	m_LightSpaceMatrix = lightProjection * lightView;
			//	m_DepthShader.SetUniform("lightSpaceMatrix", m_LightSpaceMatrix);
			//	m_CascadeLightSpaceMatrices[i] = m_LightSpaceMatrix;

			//}
		
			m_DepthShader.SetUniform("lightSpaceMatrix", m_CascadeLightSpaceMatrices[i]);
			//m_DepthShader.SetUniform("lightSpaceMatrix", m_CascadeLightSpaceMatrices[0]);

			// render scene
			for (UID const& uid : Chroma::Scene::GetShadowCastingComponentUIDs())
			{
				m_DepthShader.SetUniform("model", static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->GetWorldTransform());

				// check if mesh skinned
				bool isSkinned = static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->GetIsSkinned();
				m_DepthShader.SetUniform("isSkinned", isSkinned);
				if (isSkinned)
					static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->SetJointUniforms(m_DepthShader);

				static_cast<MeshComponent*>(Chroma::Scene::GetComponent(uid))->Draw(m_DepthShader);
			}
		}

		// Reset back to previous render settings
		glDisable(GL_DEPTH_CLAMP);
		glCullFace(GL_BACK); // reset to original culling mode
		glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		UnBind();

		//// TEST
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//// Reset back to Screen Resolution
		//m_ScreenShader->Use();

		//if(Chroma::Input::IsPressed(Chroma::Input::NUM0))
		//	m_ScreenShader->SetUniform("layer", 0);
		//if (Chroma::Input::IsPressed(Chroma::Input::NUM1))
		//	m_ScreenShader->SetUniform("layer", 1);
		//if (Chroma::Input::IsPressed(Chroma::Input::NUM2))
		//	m_ScreenShader->SetUniform("layer", 2);
		//if (Chroma::Input::IsPressed(Chroma::Input::NUM3))
		//	m_ScreenShader->SetUniform("layer", 3);

		//m_ScreenShader->SetUniform("screenTexture", 0);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D_ARRAY, m_CascadedTexureArray);

		//// setting transform uniforms
		//UpdateTransformUniforms();
		//RenderQuad();

	}


	ShadowBuffer::ShadowBuffer()
	{
		Init();
	}


	ShadowBuffer::~ShadowBuffer()
	{
	}
}
