#include "CubeMap.h"
#include <resources/TextureLoader.h>

namespace Chroma
{
	CubeMap::CubeMap(std::string cubeMapsDirectory)
	{
		m_TextureData = Chroma::TextureLoader::LoadCubeMapTextureData(cubeMapsDirectory);
	}

	CubeMap::CubeMap(std::shared_ptr<TextureData> textData)
	{
		m_TextureData = textData;
	}


	CubeMap::~CubeMap()
	{
		Destroy();
	}
}
