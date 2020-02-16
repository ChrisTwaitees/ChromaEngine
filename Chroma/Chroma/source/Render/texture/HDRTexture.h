#ifndef _CHROMA_HDRTEXTURE_
#define _CHROMA_HDRTEXTURE_

#include <texture/Texture.h>

class HDRTexture :	public Texture
{
public:
	HDRTexture(TextureData const& textData);
	HDRTexture(std::string sourcepath);
	HDRTexture() {};
	~HDRTexture();
};

#endif