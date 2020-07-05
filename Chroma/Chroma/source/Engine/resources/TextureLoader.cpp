#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <texture/stb_image.h>
#endif
#include "TextureLoader.h"



namespace Chroma
{

	std::mutex TextureLoader::m_Mutex;
	ThreadSafeQueue<std::shared_ptr<TextureData>> TextureLoader::m_2DInitalizationQueue;
	ThreadSafeQueue<std::shared_ptr<TextureData>> TextureLoader::m_HDRInitalizationQueue;

	Texture TextureLoader::Create2DTexture(std::string sourcePath)
	{
		TEXTURELOADER_LOAD

		std::shared_ptr<TextureData> newTex = std::make_shared<TextureData>();
		newTex->sourcePath = sourcePath;
		Load2DTexture(newTex);
		Initialize2DTexture(newTex);

		return Texture(newTex);
	}

	Texture TextureLoader::Create2DTexture(std::string sourcePath, std::string dir)
	{
		TEXTURELOADER_LOAD 

		std::string srcPath = dir + sourcePath;

		std::shared_ptr<TextureData> newTex = std::make_shared<TextureData>();
		newTex->sourcePath = srcPath;
		Load2DTexture(newTex);
		Initialize2DTexture(newTex);

		return Texture(newTex);
	}

	std::shared_ptr<TextureData> TextureLoader::Create2DTextureData(std::string sourcePath)
	{
		TEXTURELOADER_LOAD

		std::shared_ptr<TextureData> newTex = std::make_shared<TextureData>();
		newTex->sourcePath = sourcePath;
		Load2DTexture(newTex);
		Initialize2DTexture(newTex);

		return newTex;
	}

	void TextureLoader::Init()
	{
		// init
		CHROMA_INFO("TEXTURE LOADER :: INITIALIZED.");
	}

	void TextureLoader::OnUpdate()
	{
		// Pop and Initialize uninitialized TextureDatas
		// These are essentially listeners for completed asynchronously loaded textures

		//while (m_2DInitalizationQueue.Size() > 0)
		//	Initialize2DTexture(m_2DInitalizationQueue.TryPop().get());

		//while (m_HDRInitalizationQueue.Size() > 0)
		//	InitializeHDRTexture(m_HDRInitalizationQueue.TryPop().get());

	}

	void TextureLoader::Create2DTextureThreadSafe(std::string sourcePath, std::shared_ptr<TextureData> textureData)
	{
		// Load
		// Protect shared textureData to prevent data race
		std::lock_guard<std::mutex> lock(m_Mutex);

		// Mark not Loaded or initialized
		textureData->isInitialized = false;
		textureData->isLoaded = false;

		// Load
		textureData->imageData = stbi_load(sourcePath.c_str(), &textureData->width, &textureData->height, &textureData->nrComponents, 0);

		if (textureData->imageData)
		{
			textureData->isLoaded = true;
			// Send to initialization queue in order to be initialized on Main Thread
			//m_2DInitalizationQueue.Push(textureData);
		}
	}

	HDRTexture TextureLoader::LoadHDRTexture(std::string sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading HDR Texture from : {0} ", sourcePath);

		unsigned int ID{ 0 };
		int width{ 0 }, height{ 0 }, nrComponents{0};




		std::shared_ptr<TextureData> newTex = std::make_shared<TextureData>();
		newTex->sourcePath = sourcePath;

		// Mark not Loaded
		newTex->isInitialized = false;
		newTex->isLoaded = false;
		// Load
		stbi_set_flip_vertically_on_load(true);
		float* image = stbi_loadf(sourcePath.c_str(), &width, &height, &nrComponents, 0);
		//unsigned char* image = new unsigned char;

		// Send to initialization queue for initialization
		if (image)
		{
			CHROMA_INFO("TEXTURE LOADER :: Texture : {0} : Loaded successfully.", newTex->sourcePath);
			newTex->isLoaded = true;
		}
		else
			CHROMA_ERROR("TEXTURE LOADER :: Unable to load : {0}", newTex->sourcePath);

		// OpenGL init if loaded

		glGenTextures(1, &ID);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
		// Cleanup
		stbi_image_free(image);
		newTex->imageData = nullptr;
		newTex->isLoaded = false;
		CHROMA_TRACE("TEXTURE LOADER : Initialized HDR Texture successfully. ");

		// Mark initialized
		newTex->isInitialized = true;


		CHROMA_TRACE_UNDERLINE;

		newTex->ID = ID;
		newTex->width = width;
		newTex->height = height;
		newTex->nrComponents = nrComponents;



		return HDRTexture(newTex);
	}

	HDRTexture TextureLoader::LoadHDRTexture(std::string fileName, std::string dir)
	{
		std::string sourcePath = dir + fileName;
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading HDR Texture from : {0} ", sourcePath);

		std::shared_ptr<TextureData> newTex = std::make_shared<TextureData>();
		newTex->sourcePath = sourcePath;
		LoadHDRTexture(newTex);
		InitializeHDRTexture(newTex);

		return HDRTexture(newTex);
	}

	std::shared_ptr<TextureData> TextureLoader::LoadHDRTextureData(std::string sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading HDR Texture Data from : {0} ", sourcePath);

		std::shared_ptr<TextureData> newTex = std::make_shared<TextureData>();
		newTex->sourcePath = sourcePath;
		LoadHDRTexture(newTex);
		InitializeHDRTexture(newTex);

		return newTex;
	}

	void TextureLoader::CreateHDRTextureThreadSafe(std::string sourcePath, std::shared_ptr<TextureData> textureData)
	{
		// Load
		// Protect shared textureData to prevent data race
		std::lock_guard<std::mutex> lock(m_Mutex);

		// Mark not Loaded or initialized
		textureData->isInitialized = false;
		textureData->isLoaded = false;

		// Load
		stbi_set_flip_vertically_on_load(true);
		textureData->imageData = stbi_load(sourcePath.c_str(), &textureData->width, &textureData->height, &textureData->nrComponents, 0);


		if (textureData->imageData)
		{
			textureData->isLoaded = true;
			// Send to initialization queue in order to be initialized on Main Thread
			//m_2DInitalizationQueue.Push(textureData);
		}
	}

	CubeMap TextureLoader::LoadCubeMapTexture(std::string sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading Cube Map Texture from : {0} ", sourcePath);

		std::shared_ptr<TextureData> newTex;
		newTex->sourcePath = sourcePath;
		LoadCubeMapTexture(newTex);

		return CubeMap(newTex);
	}


	std::shared_ptr<TextureData> TextureLoader::LoadCubeMapTextureData(std::string sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading Cube Map Texture from : {0} ", sourcePath);

		std::shared_ptr<TextureData> newTex;
		newTex->sourcePath = sourcePath;
		LoadCubeMapTexture(newTex);

		return newTex;
	}

	void TextureLoader::Initialize2DTexture(std::shared_ptr<TextureData> textureData)
	{
		// Mark uninitialized
		textureData->isInitialized = false;

		// OpenGL init if loaded
		if (textureData->isLoaded)
		{
			// ID
			glGenTextures(1, &textureData->ID);

			GLenum format;
			if (textureData->nrComponents == 1)
				format = GL_RED;
			else if (textureData->nrComponents == 3)
				format = GL_RGB;
			else if (textureData->nrComponents == 4)
				format = GL_RGBA;
			GLenum wrapping;
			if (textureData->nrComponents == 1)
				wrapping = GL_REPEAT;
			else if (textureData->nrComponents == 3)
				wrapping = GL_REPEAT;
			else if (textureData->nrComponents == 4)
				wrapping = GL_REPEAT;

			glBindTexture(GL_TEXTURE_2D, textureData->ID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, textureData->width, textureData->height, 0, format, GL_UNSIGNED_BYTE, textureData->imageData);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Antisotropic filtering
			GLfloat value, max_anisotropy = 4.0f; /* don't exceed this value...*/
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &value);
			value = glm::min(value, max_anisotropy);
			glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY, value);

			//Generate mipmap
			glGenerateMipmap(GL_TEXTURE_2D);

			// Mark initialized
			textureData->isInitialized = true;

			// Cleanup
			stbi_image_free(textureData->imageData);
			textureData->imageData = nullptr;
			textureData->isLoaded = false;

			CHROMA_INFO("TEXTURE LOADER : 2D Texture Initialized successfully. ");
		}
		else
			CHROMA_ERROR("TEXTURE LOADER :: Is not Loaded, Cannot initialize : {0}", textureData->sourcePath);

		CHROMA_TRACE_UNDERLINE;
	}


	void TextureLoader::InitializeHDRTexture(std::shared_ptr<TextureData> textureData)
	{
		// Mark uninitialized
		textureData->isInitialized = false;

		// OpenGL init if loaded
		if (textureData->isLoaded)
		{
			glGenTextures(1, &textureData->ID);

			glBindTexture(GL_TEXTURE_2D, textureData->ID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, textureData->width, textureData->height, 0, GL_RGB, GL_FLOAT, textureData->imageData);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//Generate mipmap
			glGenerateMipmap(GL_TEXTURE_2D);

			// Mark initialized
			textureData->isInitialized = true;

			// Cleanup
			stbi_image_free(textureData->imageData);
			textureData->imageData = nullptr;
			textureData->isLoaded = false;
			CHROMA_TRACE("TEXTURE LOADER : Initialized HDR Texture successfully. ");
		}
		else
			CHROMA_ERROR("TEXTURE LOADER :: Is not Loaded, Cannot initialize : {0}", textureData->sourcePath);

		CHROMA_TRACE_UNDERLINE;
	}


	void TextureLoader::Load2DTexture(std::shared_ptr<TextureData> textureData)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading 2D Texture from : {0} ", textureData->sourcePath);

		// Mark not Loaded
		textureData->isInitialized = false;
		textureData->isLoaded = false;

		// Load
		textureData->imageData = stbi_load(textureData->sourcePath.c_str(), &textureData->width, &textureData->height, &textureData->nrComponents, 0);

		// Send to initialization queue for initialization
		if (textureData->imageData)
		{
			CHROMA_INFO("TEXTURE LOADER :: Texture : {0} : Loaded successfully.", textureData->sourcePath);
			textureData->isLoaded = true;
		}
		else
			CHROMA_ERROR("TEXTURE LOADER :: Unable to load : {0}", textureData->sourcePath);

		CHROMA_TRACE_UNDERLINE;
	}

	void TextureLoader::LoadHDRTexture(std::shared_ptr<TextureData> textureData)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("TEXTURE LOADER : Loading 2D HDR Texture from : {0} ", textureData->sourcePath);

		// Mark not Loaded
		textureData->isInitialized = false;
		textureData->isLoaded = false;

		// Load
		stbi_set_flip_vertically_on_load(true);
		float* imageData = nullptr;
	//	imageData = stbi_loadf(textureData->sourcePath.c_str(), &textureData->width, &textureData->height, &textureData->nrComponents, 0);

		// Send to initialization queue for initialization
		if (imageData)
		{
			CHROMA_INFO("TEXTURE LOADER :: Texture : {0} : Loaded successfully.", textureData->sourcePath);
			textureData->isLoaded = true;
		}
		else
			CHROMA_ERROR("TEXTURE LOADER :: Unable to load : {0}", textureData->sourcePath);

		CHROMA_TRACE_UNDERLINE;
	}

	void TextureLoader::LoadCubeMapTexture(std::shared_ptr<TextureData> textData)
	{
		// Mark uninitialized
		textData->isInitialized = false;
		textData->isLoaded = false;

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
		glGenTextures(1, &textData->ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textData->ID);
		// for the six faces gen 2D textures
		unsigned char* imageData;

		for (GLuint i = 0; i < faces.size(); i++)
		{
			std::string imagePath = textData->sourcePath + "/" + faces[i];
			stbi_set_flip_vertically_on_load(false);
			imageData = stbi_load(imagePath.c_str(), &textData->width, &textData->height, &textData->nrComponents, 0);
			if (imageData)
			{
				GLenum format;
				if (textData->nrComponents == 1)
					format = GL_RED;
				else if (textData->nrComponents == 3)
					format = GL_RGB;
				else if (textData->nrComponents == 4)
					format = GL_RGBA;
				GLenum wrapping;
				if (textData->nrComponents == 1)
					wrapping = GL_REPEAT;
				else if (textData->nrComponents == 3)
					wrapping = GL_REPEAT;
				else if (textData->nrComponents == 4)
					wrapping = GL_CLAMP_TO_EDGE;
				// create gtTextImage2D
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, format, textData->width, textData->height, 0, format, GL_UNSIGNED_BYTE, imageData
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
		textData->isInitialized = true;
		textData->isLoaded = true;
	}


}
