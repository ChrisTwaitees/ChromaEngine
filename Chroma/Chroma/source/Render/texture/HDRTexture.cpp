#include "HDRTexture.h"
#include <resources/TextureLoader.h>

HDRTexture::HDRTexture(TextureData const& textData)
{
	InitTextureData(textData);
}

HDRTexture::HDRTexture(std::string sourcepath)
{
	TextureData newTex = Chroma::TexureLoader::LoadHDRTextureData(sourcepath);
	InitTextureData(newTex);
}


HDRTexture::~HDRTexture()
{
	glDeleteTextures(1, &ID);
}
