#include "CubeMap.h"
#include <resources/TextureLoader.h>

CubeMap::CubeMap(std::string cubeMapsDirectory)
{
	m_TextureData = Chroma::TextureLoader::LoadCubeMapTextureData(cubeMapsDirectory);
}

CubeMap::CubeMap(TextureData const& textData)
{
	m_TextureData = textData;
}


CubeMap::~CubeMap()
{
	Destroy();
}
