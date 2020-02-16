#include "CubeMap.h"


void CubeMap::generateCubeMap()
{
	// gen id and bindShadowMapToBuffer
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	// for the six faces gen 2D textures
	int width, height, nrChannels;
	unsigned char* imageData;

	for (GLuint i = 0; i < faces.size(); i++)
	{
		std::string imagePath = dir + "/" + faces[i];
		stbi_set_flip_vertically_on_load(false);
		imageData = stbi_load(imagePath.c_str(), &width, &height, &nrChannels,0);
		if (imageData)
		{
			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;
			GLenum wrapping;
			if (nrChannels == 1)
				wrapping = GL_REPEAT;
			else if (nrChannels == 3)
				wrapping = GL_REPEAT;
			else if (nrChannels == 4)
				wrapping = GL_CLAMP_TO_EDGE;
			// create gtTextImage2D
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData
			);
			stbi_image_free(imageData);
		}
		else
		{
			CHROMA_WARN("CUBEMAP :: Texture failed to load at path: {0} ", imagePath);
			stbi_image_free(imageData);
		}
		// wrapping and filtering 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}

void CubeMap::Bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
}

CubeMap::CubeMap(std::string cubeMapsDirectory)
{
	dir = cubeMapsDirectory;
	generateCubeMap();
}


CubeMap::~CubeMap()
{
}
