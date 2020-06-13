#include "Texture.h"
#include <resources/TextureLoader.h>
#include <resources/ResourceManager.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <texture/stb_image.h>
#endif

void Texture::InitializeTexture()
{
	Chroma::TextureLoader::Initialize2DTexture(m_SharedTextureData);
}

void Texture::Bind()
{
	if (m_TextureData.isInitialized)
		glBindTexture(GL_TEXTURE_2D, m_TextureData.ID);
	else if (m_TextureData.isLoaded && m_TextureData.isInitialized == false)
		InitializeTexture();
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

//Texture::Texture(const Texture& rhs)
//{
//	CHROMA_INFO("COPY OPERATOR CALLED");
//	if (this != &rhs) { // self-assignment check expected
//		m_TextureData = rhs.m_TextureData;
//		m_SharedTextureData = rhs.m_SharedTextureData;
//	}
//}
//
//Texture& Texture::operator=(const Texture& rhs)
//{
//	CHROMA_INFO("COPY ASSIGNMENT OPERATOR CALLED");
//	if (this != &rhs) { // self-assignment check expected
//		m_TextureData = rhs.m_TextureData;
//		m_SharedTextureData = rhs.m_SharedTextureData;
//	}
//	return *this;
//}

//Texture::Texture(Texture&& rhs)
//{
//	CHROMA_INFO("MOVE CONSTRUCTOR CALLED");
//}

Texture::~Texture()
{
	//Destroy();
}

void Texture::LoadFromFile(const std::string& sourcePath)
{
	//m_SharedTextureData = std::make_shared<TextureData>();

	// Mark uninitialized
	m_SharedTextureData->isInitialized = false;
	m_TextureData.isInitialized = false;
	m_TextureData.isLoaded = false;
	m_SharedTextureData->isLoaded= false;
	m_TextureData.sourcePath = sourcePath;
	m_SharedTextureData->sourcePath = sourcePath;
	// Load
	//Chroma::ResourceManager::Load2DTexture(sourcePath, m_TextureData);
	Chroma::ResourceManager::Load2DTexture(sourcePath, m_SharedTextureData);
}

unsigned int Texture::GetID()
{
	//if (m_TextureData.isInitialized)
	//	return m_TextureData.ID;
	//else if (m_TextureData.isLoaded && m_TextureData.isInitialized == false)
	//	InitializeTexture();
	//else
	//	return 0;

	if (m_SharedTextureData->isInitialized)
		return m_SharedTextureData->ID;
	else if (m_SharedTextureData->isLoaded && m_SharedTextureData->isInitialized != true)
		InitializeTexture();
	else
		return 0;
}

void Texture::SetID(Texture& refTexture)
{
	m_TextureData.ID = refTexture.GetID();
	m_TextureData.isLoaded = refTexture.GetTextureData().isLoaded;
	m_TextureData.isInitialized = refTexture.GetTextureData().isInitialized;
}

//TextureData& TextureData::operator=(const TextureData& rhs)
//{
//	CHROMA_INFO("COPY ASSIGNMENT OPERATOR CALLED");
//	if (this != &rhs) { // self-assignment check expected
//	// RenderID
//		ID = rhs.ID;
//
//		// Dimensions
//		width = rhs.ID;
//		height = rhs.height;
//		nrComponents = rhs.nrComponents;
//
//		sourcePath = rhs.sourcePath;
//
//		if (rhs.imageData)
//		{
//			imageData = new unsigned char;
//			*imageData = *rhs.imageData;
//		}
//
//		isInitialized = rhs.isInitialized;
//		isLoaded = rhs.isLoaded;
//
//		type = rhs.type;
//	}
//	// TODO: insert return statement here
//	return *this;
//}

//TextureData::TextureData(const TextureData& rhs)
//{
//	CHROMA_INFO("COPY CONSTRUCTOR CALLED");
//	// RenderID
//	ID = rhs.ID;
//
//	// Dimensions
//	width = rhs.ID;
//	height = rhs.height;
//	nrComponents = rhs.nrComponents;
//
//	sourcePath = rhs.sourcePath;
//
//	if (rhs.imageData)
//	{
//		imageData = new unsigned char;
//		*imageData = *rhs.imageData;
//	}
//
//	isInitialized = rhs.isInitialized;
//	isLoaded = rhs.isLoaded;
//
//	type = rhs.type;
//}
//
//TextureData::TextureData(TextureData&& rhs)
//{
//	CHROMA_INFO("MOVE CONSTRUCTOR CALLED");
//}
//
//TextureData::~TextureData()
//{
//	if (imageData) 
//		delete imageData;
//
//	CHROMA_INFO("DESTRUCTOR CALLED");
//}
