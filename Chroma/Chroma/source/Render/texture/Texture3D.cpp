#include "Texture3D.h"


namespace Chroma
{
	Texture3D::Texture3D(const int _width, const int _height, const int _depth, const bool generateMipmaps)
	{
		// TextureData
		m_TextureData->width = _width;
		m_TextureData->height = _height;
		m_TextureData->depth = _depth;
		m_TextureData->type = Type::Texture::k3D;

		Init(generateMipmaps);
	}

	Texture3D::~Texture3D()
	{
	}

	void Texture3D::Bind()
	{
		glBindTexture(GL_TEXTURE_3D, m_TextureData->ID);
	}

	void Texture3D::Clear(const glm::vec4& clearColor)
	{
		GLint previousBoundTextureID;
		glGetIntegerv(GL_TEXTURE_BINDING_3D, &previousBoundTextureID);
		glBindTexture(GL_TEXTURE_3D, m_TextureData->ID);
		glClearTexImage(m_TextureData->ID, 0, GL_RGBA, GL_FLOAT, &clearColor);
		glBindTexture(GL_TEXTURE_3D, previousBoundTextureID);
	}

	void Texture3D::Init(const bool generateMipMaps)
	{
		// Generate texture on GPU.
		glGenTextures(1, &m_TextureData->ID);
		glBindTexture(GL_TEXTURE_3D, m_TextureData->ID);

		// Parameter options.
		const unsigned int wrap = GL_CLAMP_TO_BORDER;
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap);

		const unsigned int filter = GL_LINEAR_MIPMAP_LINEAR;
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Upload texture buffer.
		const int levels = 7;
		glTexStorage3D(GL_TEXTURE_3D, levels, GL_RGBA16, m_TextureData->width, m_TextureData->height, m_TextureData->depth);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16, m_TextureData->width, m_TextureData->height, m_TextureData->depth, 0, GL_RGBA, GL_FLOAT, &m_TextureBuffer);
		if (generateMipMaps) glGenerateMipmap(GL_TEXTURE_3D);
		glBindTexture(GL_TEXTURE_3D, 0);

		// Marking initialized
		m_TextureData->isInitialized = true;
	}
}