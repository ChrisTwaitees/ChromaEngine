#ifndef _CHROMA_HDRTEXTURE_
#define _CHROMA_HDRTEXTURE_

#include <texture/Texture.h>

class HDRTexture :	public Texture
{
	void generateTexture() override;
public:
	HDRTexture(std::string sourcepath);
	~HDRTexture();
};

#endif