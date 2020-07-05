#ifndef _CHROMA_TEXTURE_LOADER_H
#define _CHROMA_TEXTURE_LOADER_H

#include <common/PrecompiledHeader.h>
#include <threading/ThreadSafeQueue.h>
#include <texture/Texture.h>
#include <texture/HDRTexture.h>
#include <texture/CubeMap.h>
#include <time/Timer.h>

namespace Chroma
{
	class TextureLoader
	{
	public:
		// Init
		static void Init();

		// update
		static void OnUpdate();

		static Texture Create2DTexture(std::string sourcePath);
		static Texture Create2DTexture(std::string sourcePath, std::string dir);
		static std::shared_ptr<TextureData> Create2DTextureData(std::string sourcePath);
		// async
		//static void Create2DTextureThreadSafe(std::string sourcePath, TextureData& textureData);
		static void Create2DTextureThreadSafe(std::string sourcePath, std::shared_ptr<TextureData> textureData);

		static HDRTexture LoadHDRTexture(std::string sourcePath);
		static HDRTexture LoadHDRTexture(std::string sourcePath, std::string dir);
		static std::shared_ptr<TextureData> LoadHDRTextureData(std::string sourcePath);
		// async
		//static void CreateHDRTextureThreadSafe(std::string sourcePath, TextureData& textureData);
		static void CreateHDRTextureThreadSafe(std::string sourcePath, std::shared_ptr<TextureData> textureData);

		static CubeMap LoadCubeMapTexture(std::string directory);
		static std::shared_ptr<TextureData> LoadCubeMapTextureData(std::string sourcePath);

		// Initialize
		static void Initialize2DTexture(std::shared_ptr<TextureData> textureData);
		static void InitializeHDRTexture(std::shared_ptr<TextureData> textureData);
	private:

		// main thread
		static void Load2DTexture(std::shared_ptr<TextureData> textureData);
		static void LoadHDRTexture(std::shared_ptr<TextureData> textureData);
		static void LoadCubeMapTexture(std::shared_ptr<TextureData> textureData);

		// threading
		static std::mutex m_Mutex;

		// intialization queue
		static ThreadSafeQueue<std::shared_ptr<TextureData>> m_2DInitalizationQueue;
		static ThreadSafeQueue<std::shared_ptr<TextureData>> m_HDRInitalizationQueue;

		// Debug
# define TEXTURELOADER_LOAD	  CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE("TEXTURE LOADER :: Loading Texture from: {0}", sourcePath);    CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE_TIMER_SCOPE( "TEXTURE LOADER :: LOAD " );

	};
}



#endif