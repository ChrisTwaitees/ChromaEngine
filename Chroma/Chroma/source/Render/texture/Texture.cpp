#include "Texture.h"
#include <resources/TextureLoader.h>

void Texture::InitTextureData(TextureData const& textData)
{
	m_SourcePath = textData.sourcePath;
	ID = textData.ID;
	width = textData.width;
	height = textData.height;
	nrComponents = textData.nrComponents;
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Destroy()
{
	CHROMA_TRACE("Removing Texture : {0}", ID);
	glDeleteTextures(1, &ID);
}

void Texture::ClearTexureMemory(unsigned int const& TextureID)
{
	CHROMA_TRACE("Removing Texture : {0}", TextureID);
	glDeleteTextures(1, &TextureID);
}

Texture::Texture(unsigned int newID)
{
	ID = newID;
}

Texture::Texture(std::string sourcepath)
{
	m_SourcePath = sourcepath;
	TextureData newTex = Chroma::TexureLoader::Load2DTextureData(sourcepath);
	InitTextureData(newTex);
}

Texture::Texture(TextureData textData)
{
	InitTextureData(textData);
}

Texture::~Texture()
{
}