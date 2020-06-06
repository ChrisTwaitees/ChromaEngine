#ifndef _CHROMA_RESOURCE_MANAGER_H
#define _CHROMA_RESOURCE_MANAGER_H

#include <resources/AnimationLoader.h>
#include <resources/ModelLoader.h>
#include <resources/TextureLoader.h>

#include <jobsystem/JobSystem.h>
#include <time/Timer.h>

namespace Chroma
{
	class ResourceManager
	{
	public:
		// Textures
		static std::vector<Texture> LoadTextures(std::string const& sourcePath);
		static Texture LoadTexture(std::string const& sourcePath);
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


	};
# define RESOURCEMANAGER_LOADMODEL  	  CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE("RESOURCE MANAGER :: Loading Models from: {0}", sourcePath);     CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE_TIMER_SCOPE( "RESOURCE MANAGER :: MESH LOAD " );
# define RESOURCEMANAGER_LOADTEXTURE	  CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE("RESOURCE MANAGER :: Loading Texture from: {0}", sourcePath);    CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE_TIMER_SCOPE( "RESOURCE MANAGER :: TEXTURE LOAD " );
# define RESOURCEMANAGER_LOADANIMATION	  CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE("RESOURCE MANAGER :: Loading Animation from: {0}", sourcePath);  CHROMA_TRACE_UNDERLINE;	CHROMA_TRACE_TIMER_SCOPE( "RESOURCE MANAGER :: ANIMATION LOAD " );
}

#endif