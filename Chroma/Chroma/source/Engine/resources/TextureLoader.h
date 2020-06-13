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
		// update
		static void Update();

		static Texture Create2DTexture(std::string sourcePath);
		static Texture Create2DTexture(std::string sourcePath, std::string dir);
		static TextureData Create2DTextureData(std::string sourcePath);
		// async
		static void Create2DTextureThreadSafe(std::string sourcePath, TextureData& textureData);

		static HDRTexture LoadHDRTexture(std::string sourcePath);
		static HDRTexture LoadHDRTexture(std::string sourcePath, std::string dir);
		static TextureData LoadHDRTextureData(std::string sourcePath);
		// async
		static void CreateHDRTextureThreadSafe(std::string sourcePath, TextureData& textureData);

		static CubeMap LoadCubeMapTexture(std::string directory);
		static TextureData LoadCubeMapTextureData(std::string sourcePath);

		// Initialize
		static void Initialize2DTexture(TextureData& textureData);
		static void InitializeHDRTexture(TextureData& textureData);
	private:

		// main thread
		static void Load2DTexture(TextureData& textureData);
		static void LoadHDRTexture(TextureData& textureData);
		static void LoadCubeMapTexture(TextureData& textureData);

		static void Initialize2DTexture(TextureData* textureData);
		static void InitializeHDRTexture(TextureData* textureData);

		// threading
		static std::mutex m_Mutex;

		// intialization queue
		static ThreadSafeQueue<TextureData> m_2DInitalizationQueue;
		static ThreadSafeQueue<TextureData> m_HDRInitalizationQueue;

		// Debug
# define TEXTURELOADER_LOAD	  CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE("TEXTURE LOADER :: Loading Texture from: {0}", sourcePath);    CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE_TIMER_SCOPE( "TEXTURE LOADER :: LOAD " );

	};
}



#endif