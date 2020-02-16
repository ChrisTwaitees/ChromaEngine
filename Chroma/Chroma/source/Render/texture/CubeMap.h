#ifndef _CHROMA_CUBEMAP_
#define _CHROMA_CUBEMAP_

#include <texture/Texture.h>


class CubeMap : public Texture
{

public:
	CubeMap(std::string cubeMapsDirectory);
	CubeMap(TextureData const& textData);
	CubeMap() {};
	~CubeMap();
};

#endif