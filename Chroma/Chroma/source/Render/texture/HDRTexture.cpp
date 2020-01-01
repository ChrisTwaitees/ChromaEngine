
#include "stb_image.h"
#include "HDRTexture.h"


void HDRTexture::generateTexture()
{
	type = TYPE::HDR;
	glGenTextures(1, &ID);
	stbi_set_flip_vertically_on_load(true);
	float* data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
		CHROMA_WARN("TEXTURE :: Failed to Load HDR Texture : {0}", path);

}

HDRTexture::HDRTexture(std::string sourcepath)
{
	path = sourcepath;
	generateTexture();
}


HDRTexture::~HDRTexture()
{
}
