#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"
#include <iostream>
#include "Texture.h"


Texture::Texture(const char* textureSourcePath, int imageFormat)
{
	// generating a texture ID and Bind
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// because opengl expect 0.0 of y to be at the bototm but most images it's at the top so we flip it vertically before load
	stbi_set_flip_vertically_on_load(true);
	// retrieve image
	unsigned char* imageData = stbi_load(textureSourcePath, &width, &height, &nrChannels, 0);
	if (imageData)
	{
		// generate texture using the image data
		glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE ,imageData );
		// we can provide custom mip maps or ask openGL to create some for us
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::TEXTURE::FAILED TO LOAD FROM : " << textureSourcePath << std::endl;
	}
	// free image memory
	stbi_image_free(imageData);
}


void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

Texture::~Texture()
{
}
