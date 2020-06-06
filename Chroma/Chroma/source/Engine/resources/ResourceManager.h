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
		static void LoadModels(std::string const& sourcePath, std::vector<MeshData>* meshDatas);
		static MeshData LoadModel(std::string const& sourcePath);
		static void LoadModel(std::string const& sourcePath, MeshData* meshData);
		// Animations
		static std::vector<Take> LoadAnimations(std::string const& sourcePath);
		static Take LoadAnimation(std::string const& sourcePath);

	};
# define RESOURCEMANAGER_LOADMODEL  	CHROMA_TRACE_UNDERLINE;		CHROMA_TRACE("RESOURCE MANAGER :: Loading Models from: {0}", sourcePath); CHROMA_TRACE_TIMER_SCOPE("Load Model"); CHROMA_TRACE_UNDERLINE;	
# define RESOURCEMANAGER_LOADTEXTURE	CHROMA_TRACE_UNDERLINE;		CHROMA_TRACE("RESOURCE MANAGER :: Loading Texture from: {0}", sourcePath); CHROMA_TRACE_TIMER_SCOPE("Load Texture"); CHROMA_TRACE_UNDERLINE;	
# define RESOURCEMANAGER_LOADANIMATION	CHROMA_TRACE_UNDERLINE;		CHROMA_TRACE("RESOURCE MANAGER :: Loading Animation from: {0}", sourcePath); CHROMA_TRACE_TIMER_SCOPE("Load Animation"); CHROMA_TRACE_UNDERLINE;	
}

#endif