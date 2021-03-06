#include "IBL.h"
#include <core/Application.h>

namespace Chroma
{
	void IBL::Init()
	{
		glDisable(GL_CULL_FACE); // double sided rendering
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		// env cube map
		GenerateEnvCubeMap();

		// irradiance map
		GenerateIrradianceMap();

		// prefilter importance map
		GeneratePrefilterMap();

		// brdf LUT map
		GenerateBRDFLUTMap();

		glEnable(GL_CULL_FACE);
		glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}

	void IBL::ClearTextureBuffers()
	{
		Texture::ClearTexureMemory(m_brdfLUTTexture);
		Texture::ClearTexureMemory(m_envCubeMap);
		Texture::ClearTexureMemory(m_prefilterMap);
		Texture::ClearTexureMemory(m_irradianceMap);
	}

	void IBL::GenerateEnvCubeMap()
	{
		// init capture buffers
		glGenFramebuffers(1, &m_captureFBO);
		glGenRenderbuffers(1, &m_captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_captureRBO);

		// init env cube map
		glGenTextures(1, &m_envCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubeMap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
				512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// convert HDR equirectangular environment map to cubemap equivalent
		m_envMapShader.Use();
		m_envMapShader.SetUniform("equirectangularMap", 0);
		m_envMapShader.SetUniform("projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_HDRtexture.GetID());

		glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
		for (unsigned int i = 0; i < 6; ++i)
		{
			m_envMapShader.SetUniform("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_envCubeMap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// draw cube
			m_captureCube.BindDrawVAO();
		}
		glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void IBL::GenerateIrradianceMap()
	{
		// generate Texture
		glGenTextures(1, &m_irradianceMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0,
				GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Update the capture buffers to the new resolution
		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

		// convolute the irradiance cubemap
		m_irradienceMapShader.Use();
		m_irradienceMapShader.SetUniform("environmentMap", 0);
		m_irradienceMapShader.SetUniform("projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubeMap);

		glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
		for (unsigned int i = 0; i < 6; ++i)
		{
			m_irradienceMapShader.SetUniform("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_irradianceMap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_captureCube.BindDrawVAO();
		}
		glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void IBL::GeneratePrefilterMap()
	{
		// generate texture
		glGenTextures(1, &m_prefilterMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		// Update the capture buffers to the new resolution
		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 128, 128);

		// convolute the prefilter cubemap
		m_prefilterMapShader.Use();
		m_prefilterMapShader.SetUniform("environmentMap", 0);
		m_prefilterMapShader.SetUniform("projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubeMap);

		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
		{
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth = 128 * std::pow(0.5, mip);
			unsigned int mipHeight = 128 * std::pow(0.5, mip);
			glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			m_prefilterMapShader.SetUniform("roughness", roughness);
			for (unsigned int i = 0; i < 6; ++i)
			{
				m_prefilterMapShader.SetUniform("view", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_prefilterMap, mip);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				m_captureCube.BindDrawVAO();
			}
		}
		glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void IBL::GenerateBRDFLUTMap()
	{
		glGenTextures(1, &m_brdfLUTTexture);

		// pre-allocate enough memory for the LUT texture.
		glBindTexture(GL_TEXTURE_2D, m_brdfLUTTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
		// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brdfLUTTexture, 0);

		glViewport(0, 0, 512, 512);
		m_brdfShader.Use();
		m_brdfShader.SetUniform("scale", glm::vec2(1.0));
		m_brdfShader.SetUniform("offset", glm::vec2(0.0));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_capturePlane.BindDrawVAO();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());

	}


	void IBL::Draw()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_HDRtexture.GetID());
		m_captureCube.Draw(m_envMapShader);
	}

	void IBL::SetIBLTexture(HDRTexture newHDRTexture)
	{
		m_HDRtexture = newHDRTexture;
		Init();
	}

	void IBL::LoadIBL(std::string const& newHDRTexturePath)
	{
		// clear old texture
		m_HDRtexture.Destroy();

		// new texture
		m_HDRtexture = Chroma::TextureLoader::LoadHDRTexture(newHDRTexturePath);

		// clear existing texture buffers
		ClearTextureBuffers();

		// initialize
		Init();
	}

	IBL::IBL()
	{
		m_HDRtexture = Chroma::TextureLoader::LoadHDRTexture("resources/hdri/BasketballCourt_3k.hdr");
		Init();
	}
}