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

Texture::Texture(unsigned int newID)
{
	ID = newID;
}

Texture::Texture(std::string sourcepath)
{
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