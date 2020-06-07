#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <texture/stb_image.h>
#endif
#include "TextureLoader.h"



namespace Chroma
{
	Texture TexureLoader::Load2DTexture(std::string sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading 2D Texture from : {0} ", sourcePath);

		TextureData newTex;
		newTex.sourcePath = sourcePath;
		Generate2DTexture(newTex);

		return Texture(newTex);
	}
	Texture TexureLoader::Load2DTexture(std::string fileName, std::string dir)
	{
		std::string sourcePath = dir + fileName;
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading 2D Texture from : {0} ", sourcePath);

		TextureData newTex;
		newTex.sourcePath = sourcePath;
		Generate2DTexture(newTex);

		return Texture(newTex);
	}

	TextureData TexureLoader::Load2DTextureData(std::string sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading 2D Texture Data from : {0} ", sourcePath);

		TextureData newTex;
		newTex.sourcePath = sourcePath;
		Generate2DTexture(newTex);

		return newTex;
	}

	void TexureLoader::Load2DTextureDataThreadSafe(std::string sourcePath, TextureData& textureData)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading 2D Texture from : {0} ", sourcePath);
		
		// Mark not Loaded
		textureData.isInitialized = false;
		textureData.isLoaded = false;

		// Load
		textureData.imageData = stbi_load(sourcePath.c_str(), &textureData.width, &textureData.height, &textureData.nrComponents, 0);

		// Mark ready to initialize
		textureData.isLoaded = true;

		CHROMA_TRACE_UNDERLINE;
	}


	void TexureLoader::LoadHDRTextureDataThreadSafe(std::string sourcePath, TextureData& textureData)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading 2D Texture from : {0} ", sourcePath);

		// Mark not Loaded
		textureData.isInitialized = false;
		textureData.isLoaded = false;

		// Load
		stbi_set_flip_vertically_on_load(true);
		textureData.imageData = stbi_load(sourcePath.c_str(), &textureData.width, &textureData.height, &textureData.nrComponents, 0);

		// Mark ready to initialize
		textureData.isLoaded = true;

		CHROMA_TRACE_UNDERLINE;
	}

	HDRTexture TexureLoader::LoadHDRTexture(std::string sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading HDR Texture from : {0} ", sourcePath);

		TextureData newTex;
		newTex.sourcePath = sourcePath;
		GenerateHDRTexture(newTex);

		return HDRTexture(newTex);
	}

	HDRTexture TexureLoader::LoadHDRTexture(std::string fileName, std::string dir)
	{
		std::string sourcePath = dir + fileName;
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading HDR Texture from : {0} ", sourcePath);

		TextureData newTex;
		newTex.sourcePath = sourcePath;
		GenerateHDRTexture(newTex);

		return HDRTexture(newTex);
	}

	TextureData TexureLoader::LoadHDRTextureData(std::string sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading HDR Texture Data from : {0} ", sourcePath);

		TextureData newTex;
		newTex.sourcePath = sourcePath;
		GenerateHDRTexture(newTex);

		return newTex;
	}

	CubeMap TexureLoader::LoadCubeMapTexture(std::string sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading Cube Map Texture from : {0} ", sourcePath);

		TextureData newTex;
		newTex.sourcePath = sourcePath;
		GenerateCubeMapTexture(newTex);

		return CubeMap(newTex);
	}


	TextureData TexureLoader::LoadCubeMapTextureData(std::string sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading Cube Map Texture from : {0} ", sourcePath);

		TextureData newTex;
		newTex.sourcePath = sourcePath;
		GenerateCubeMapTexture(newTex);

		return newTex;
	}

	void TexureLoader::Generate2DTexture(TextureData& textData)
	{
		// Mark uninitialized
		textData.isInitialized = false;
		textData.isLoaded = false;

		// Texture
		unsigned char* data = stbi_load(textData.sourcePath.c_str(), &textData.width, &textData.height, &textData.nrComponents, 0);
		if (data)
		{
			// ID
			glGenTextures(1, &textData.ID);

			GLenum format;
			if (textData.nrComponents == 1)
				format = GL_RED;
			else if (textData.nrComponents == 3)
				format = GL_RGB;
			else if (textData.nrComponents == 4)
				format = GL_RGBA;
			GLenum wrapping;
			if (textData.nrComponents == 1)
				wrapping = GL_REPEAT;
			else if (textData.nrComponents == 3)
				wrapping = GL_REPEAT;
			else if (textData.nrComponents == 4)
				wrapping = GL_REPEAT;

			glBindTexture(GL_TEXTURE_2D, textData.ID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, textData.width, textData.height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Antisotropic filtering
			GLfloat value, max_anisotropy = 4.0f; /* don't exceed this value...*/
			glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY, &value);
			value = glm::min(value, max_anisotropy);
			glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY, value);

			// Mark initialized
			textData.isInitialized = true;
			textData.isLoaded = true;

			stbi_image_free(data);
			CHROMA_TRACE("TEXTURE LOADER : Loaded 2D Texture successfully. ");
		}
		else
			CHROMA_ERROR("TEXTURE LOADER :: Failed to load from path : {0}", textData.sourcePath);
			CHROMA_TRACE_UNDERLINE;

	}
	void TexureLoader::GenerateHDRTexture(TextureData& textData)
	{
		// Mark uninitialized
		textData.isInitialized = false;
		textData.isLoaded = false;

		stbi_set_flip_vertically_on_load(true);
		float* data = stbi_loadf(textData.sourcePath.c_str(), &textData.width, &textData.height, &textData.nrComponents, 0);
		if (data)
		{
			glGenTextures(1, &textData.ID);

			glBindTexture(GL_TEXTURE_2D, textData.ID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, textData.width, textData.height, 0, GL_RGB, GL_FLOAT, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
			CHROMA_TRACE("TEXTURE LOADER : Loaded HDR Texture successfully. ");

			// Mark initialized
			textData.isInitialized = true;
			textData.isLoaded = true;
		}
		else
			CHROMA_ERROR("TEXTURE :: Failed to Load HDR Texture : {0}", textData.sourcePath);
		CHROMA_TRACE_UNDERLINE;

	}
	void TexureLoader::GenerateCubeMapTexture(TextureData& textData)
	{
		// Mark uninitialized
		textData.isInitialized = false;
		textData.isLoaded = false;

		std::vector<std::string> faces
		{
			"right.jpg",
			"left.jpg",
			"top.jpg",
			"bottom.jpg",
			"front.jpg",
			"back.jpg"
		};
		// gen id and bindShadowMapToBuffer
		glGenTextures(1, &textData.ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textData.ID);
		// for the six faces gen 2D textures
		unsigned char* imageData;

		for (GLuint i = 0; i < faces.size(); i++)
		{
			std::string imagePath = textData.sourcePath + "/" + faces[i];
			stbi_set_flip_vertically_on_load(false);
			imageData = stbi_load(imagePath.c_str(), &textData.width, &textData.height, &textData.nrComponents, 0);
			if (imageData)
			{
				GLenum format;
				if (textData.nrComponents == 1)
					format = GL_RED;
				else if (textData.nrComponents == 3)
					format = GL_RGB;
				else if (textData.nrComponents == 4)
					format = GL_RGBA;
				GLenum wrapping;
				if (textData.nrComponents == 1)
					wrapping = GL_REPEAT;
				else if (textData.nrComponents == 3)
					wrapping = GL_REPEAT;
				else if (textData.nrComponents == 4)
					wrapping = GL_CLAMP_TO_EDGE;
				// create gtTextImage2D
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, format, textData.width, textData.height, 0, format, GL_UNSIGNED_BYTE, imageData
				);
				stbi_image_free(imageData);
			}
			else
			{
				CHROMA_WARN("TEXTURE LOADER :: CubeMap failed to load at path: {0} ", imagePath);
				stbi_image_free(imageData);
			}
			// wrapping and filtering 
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

		// Mark uninitialized
		textData.isInitialized = true;
		textData.isLoaded = true;

	}
}
