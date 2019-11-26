#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"
#include "Texture.h"



int Texture::loadFromFile(std::string filename, std::string dir)
{
	path = dir + '/' + filename;
	generateTexture();
	return ID;
}

int Texture::loadFromFile(std::string sourcepath)
{
	path = sourcepath;
	generateTexture();
	return ID;
}

void Texture::generateTexture()
{
	glGenTextures(1, &ID);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		GLenum wrapping;
		if (nrComponents == 1)
			wrapping = GL_REPEAT;
		else if (nrComponents == 3)
			wrapping = GL_REPEAT;
		else if (nrComponents == 4)
			wrapping = GL_CLAMP_TO_EDGE;

		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
		std::cout << "Texture failed to load at path: " << path << std::endl;
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

Texture::Texture(unsigned int newID)
{
	ID = newID;
}

Texture::Texture(std::string sourcepath)
{
	path = sourcepath;
	generateTexture();
}

Texture::Texture()
{
}

Texture::~Texture()
{
}