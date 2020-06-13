#ifndef CHROMA_HDRTEXTURE_H
#define CHROMA_HDRTEXTURE_H

#include <texture/Texture.h>

class HDRTexture :	public Texture
{
public:
	HDRTexture(TextureData const& textData);
	HDRTexture(const std::string& sourcepath);
	HDRTexture() {};
	~HDRTexture();

	// load	
	void LoadFromFile(const std::string& sourcePath) override;

private:
	// Init
	void InitializeTexture() override;
};

#endif //CHROMA_HDRTEXTURE_H