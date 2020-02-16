#include "CubeMap.h"
#include <resources/TextureLoader.h>

CubeMap::CubeMap(std::string cubeMapsDirectory)
{
	TextureData newTex = Chroma::TexureLoader::LoadCubeMapTextureData(cubeMapsDirectory);
	InitTextureData(newTex);
}

CubeMap::CubeMap(TextureData const& textData)
{
	InitTextureData(textData);
}


CubeMap::~CubeMap()
{
	glDeleteTextures(1, &ID);
}
