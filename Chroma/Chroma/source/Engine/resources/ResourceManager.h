#ifndef _CHROMA_RESOURCE_MANAGER_H
#define _CHROMA_RESOURCE_MANAGER_H

#include <resources/AnimationLoader.h>
#include <resources/ModelLoader.h>
#include <resources/TextureLoader.h>

#include <jobsystem/JobSystem.h>

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
		// Animations
		static std::vector<Take> LoadAnimations(std::string const& sourcePath);
		static Take LoadAnimation(std::string const& sourcePath);

	};
}

#endif