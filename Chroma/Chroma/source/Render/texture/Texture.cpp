#include "Texture.h"
#include <resources/TextureLoader.h>
#include <resources/ResourceManager.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <texture/stb_image.h>
#endif

void Texture::SetupTexture()
{
	// Mark uninitialized
	m_TextureData.isInitialized = false;


	// Check for Data
	if (m_TextureData.imageData)
	{
		// ID
		glGenTextures(1, &m_TextureData.ID);

		GLenum format;
		if (m_TextureData.nrComponents == 1)
			format = GL_RED;
		else if (m_TextureData.nrComponents == 3)
			format = GL_RGB;
		else if (m_TextureData.nrComponents == 4)
			format = GL_RGBA;
		GLenum wrapping;
		if (m_TextureData.nrComponents == 1)
			wrapping = GL_REPEAT;
		else if (m_TextureData.nrComponents == 3)
			wrapping = GL_REPEAT;
		else if (m_TextureData.nrComponents == 4)
			wrapping = GL_REPEAT;

		glBindTexture(GL_TEXTURE_2D, m_TextureData.ID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_TextureData.width, m_TextureData.height, 0, format, GL_UNSIGNED_BYTE, m_TextureData.imageData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Antisotropic filtering
		GLfloat value, max_anisotropy = 4.0f; /* don't exceed this value...*/
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &value);
		value = glm::min(value, max_anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY, value);

		// Mark initialized
		m_TextureData.isInitialized = true;

		// Cleanup
		stbi_image_free(m_TextureData.imageData);

		// Log
		CHROMA_INFO("2D Texture Initialized : {0}", m_TextureData.sourcePath);
	}
	else
	{
		CHROMA_ERROR("ImageData not Loaded from : {0}", m_TextureData.sourcePath);
	}
}


void Texture::Bind()
{
	if (m_TextureData.isInitialized)
		glBindTexture(GL_TEXTURE_2D, m_TextureData.ID);
	else if (m_TextureData.isLoaded && m_TextureData.isInitialized == false)
		SetupTexture();
}

void Texture::Destroy()
{
	CHROMA_TRACE("Removing Texture : {0}", m_TextureData.ID);
	glDeleteTextures(1, &m_TextureData.ID);
}

void Texture::ClearTexureMemory(unsigned int const& TextureID)
{
	CHROMA_TRACE("Removing Texture : {0}", TextureID);
	glDeleteTextures(1, &TextureID);
}

Texture::Texture(unsigned int newID)
{
	m_TextureData.ID = newID;
}

Texture::Texture(std::string sourcepath)
{
	LoadFromFile(sourcepath);
}

Texture::Texture(std::string sourcePath, Chroma::Type::Texture type)
{
	m_TextureData.type = type;

	LoadFromFile(sourcePath);
}

Texture::Texture(TextureData const& textData)
{
	m_TextureData = textData;
}

bool Texture::operator<(const Texture& rhs) const
{
	return true;
}

Texture::~Texture()
{
	//Destroy();
}

void Texture::LoadFromFile(const std::string& sourcePath)
{
	// Mark uninitialized
	m_TextureData.isInitialized = false;
	m_TextureData.isLoaded = false;
	// Load
	m_TextureData = Chroma::TexureLoader::Load2DTextureData(sourcePath);
	//Chroma::ResourceManager::Load2DTexture(sourcePath, &m_TextureData);
}

unsigned int Texture::GetID()
{
	if (m_TextureData.isInitialized)
		return m_TextureData.ID;
	else if (m_TextureData.isLoaded && m_TextureData.isInitialized == false)
		SetupTexture();
	else
		return 0;
}

void Texture::SetID(Texture& refTexture)
{
	m_TextureData.ID = refTexture.GetID();
	m_TextureData.isLoaded = refTexture.GetTextureData().isLoaded;
	m_TextureData.isInitialized = refTexture.GetTextureData().isInitialized;
}

TextureData& TextureData::operator=(const TextureData& rhs)
{
	CHROMA_INFO("COPY CONSTRUCTOR CALLED");
	if (this != &rhs) { // self-assignment check expected
		if (rhs.imageData)
		{
			this->imageData = new unsigned char;
			*this->imageData = *rhs.imageData;
			//memcpy(imageData, rhs.imageData, sizeof(rhs.imageData));

		}
		height = rhs.height;
		width = rhs.width;
		nrComponents = rhs.nrComponents;
		isInitialized = rhs.isInitialized;
		isLoaded = rhs.isLoaded;
		ID = rhs.ID;
		//sourcePath = rhs.sourcePath;
		//this->type = Chroma::Type::Texture::kAlbedo;
	}

	// TODO: insert return statement here
	return *this;
}
