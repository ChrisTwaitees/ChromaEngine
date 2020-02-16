#include "ResourceManager.h"
#include <texture/HDRTexture.h>
#include <texture/CubeMap.h>

namespace Chroma
{
	std::vector<Texture> ResourceManager::LoadTextures(std::string const& sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("RESOURCE MANAGER :: Loading Texture from: {0}", sourcePath);
		return std::vector<Texture>();
	}
	Texture ResourceManager::LoadTexture(std::string const& sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("RESOURCE MANAGER :: Loading Texture from: {0}", sourcePath);
		return Chroma::TexureLoader::Load2DTexture(sourcePath);
	}
	std::vector<MeshData> ResourceManager::LoadModels(std::string const& sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("RESOURCE MANAGER :: Loading Models from: {0}", sourcePath);
		std::vector<MeshData> meshList;
		//std::reference_wrapper<std::vector<MeshData>> refMeshList = std::ref(meshList);
		//Chroma::JobSystem::Execute([sourcePath, refMeshList] { ModelLoader::LoadThreadSafe(sourcePath, refMeshList); });
		//Chroma::JobSystem::Wait();
		ModelLoader::LoadThreadSafe(sourcePath, meshList);
		if (meshList.size() > 0)
		{
			return meshList;
		}
		else
		{
			CHROMA_ERROR("RESOURCE MANAGER :: LoadModel :: Cannot find model at : {0}", sourcePath);
			return meshList;
		}
	}
	MeshData ResourceManager::LoadModel(std::string const& sourcePath)
	{
		CHROMA_TRACE_UNDERLINE;
		CHROMA_TRACE("RESOURCE MANAGER :: Loading Model from: {0}", sourcePath);
		std::vector<MeshData> meshList;
		//std::reference_wrapper<std::vector<MeshData>> refMeshList = std::ref(meshList);
		//Chroma::JobSystem::Execute([sourcePath, refMeshList] { ModelLoader::LoadThreadSafe(sourcePath, refMeshList); });
		//Chroma::JobSystem::Wait();
		ModelLoader::LoadThreadSafe(sourcePath, meshList);
		if (meshList.size() > 0)
		{
			return meshList[0];
		}
		else
		{
			CHROMA_ERROR("RESOURCE MANAGER :: LoadModel :: Cannot find model at : {0}", sourcePath);
			return MeshData();
		}
	}
	std::vector<Take> ResourceManager::LoadAnimations(std::string const& sourcePath)
	{
		return std::vector<Take>();
	}
	Take ResourceManager::LoadAnimation(std::string const& sourcePath)
	{
		return Take();
	}
}


