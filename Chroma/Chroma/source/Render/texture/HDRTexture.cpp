#include "HDRTexture.h"
#include <resources/ResourceManager.h>


#ifndef STB_IMAGE_IMPLEMENTATION
#include <texture/stb_image.h>
#endif

HDRTexture::HDRTexture(TextureData const& textData)
{
	m_TextureData = textData;
}

HDRTexture::HDRTexture(const std::string& sourcepath)
{
	m_TextureData.type = Chroma::Type::Texture::kHDR;
	LoadFromFile(sourcepath);
}


HDRTexture::~HDRTexture()
{
	//Destroy();
}

void HDRTexture::LoadFromFile(const std::string& sourcePath)
{
	// Mark uninitialized
	m_TextureData.isInitialized = false;
	m_TextureData.isLoaded = false;
	m_TextureData.sourcePath = sourcePath;
	// Load
	Chroma::ResourceManager::LoadHDRTexture(sourcePath, m_TextureData);
}

void HDRTexture::InitializeTexture()
{
	Chroma::TextureLoader::InitializeHDRTexture(m_TextureData);
}
