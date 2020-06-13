#ifndef _CHROMA_RESOURCE_MANAGER_H
#define _CHROMA_RESOURCE_MANAGER_H

#include <resources/AnimationLoader.h>
#include <resources/ModelLoader.h>
#include <resources/TextureLoader.h>

#include <time/Timer.h>

namespace Chroma
{
	class ResourceManager
	{
	public:
		// Update
		static void Update();

		// Textures
		static Texture Create2DTexture(std::string const& sourcePath);
		static void Load2DTexture(const std::string& sourcePath, std::shared_ptr<TextureData> textureData);
		static void Load2DTexture(const std::string& sourcePath, TextureData& textureData);
		static void LoadHDRTexture(const std::string& sourcePath, TextureData& textureData);

		// Models
		static std::vector<MeshData> LoadModels(std::string const& sourcePath);
		static MeshData LoadModel(std::string const& sourcePath);
		static void LoadModels(std::string const& sourcePath, std::vector<MeshData>* meshDatas);
		static void LoadModel(std::string const& sourcePath, MeshData* meshData);

		// Animations
		static std::vector<Take> LoadAnimations(std::string const& sourcePath);
		static Take LoadAnimation(std::string const& sourcePath);

	private:
		// async
		static std::vector<std::future<void>> m_Futures;
		static std::mutex m_Mutex;

		// async - model
		static void LoadModelAsync(const std::string& sourcePath, MeshData* meshdata);
		static void LoadModelsAsync(const std::string& sourcePath, std::vector<MeshData>* meshdatas);

		static void Test(std::string test, std::shared_ptr<TextureData> textureData);


	};
# define RESOURCEMANAGER_LOADMODEL  	  CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE("RESOURCE MANAGER :: Loading Models from: {0}", sourcePath);     CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE_TIMER_SCOPE( "RESOURCE MANAGER :: MESH LOAD " );
# define RESOURCEMANAGER_LOADTEXTURE	  CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE("RESOURCE MANAGER :: Loading Texture from: {0}", sourcePath);    CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE_TIMER_SCOPE( "RESOURCE MANAGER :: TEXTURE LOAD " );
# define RESOURCEMANAGER_LOADANIMATION	  CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE("RESOURCE MANAGER :: Loading Animation from: {0}", sourcePath);  CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE_TIMER_SCOPE( "RESOURCE MANAGER :: ANIMATION LOAD " );
}

#endif