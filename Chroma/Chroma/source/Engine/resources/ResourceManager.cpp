#include "ResourceManager.h"


namespace Chroma
{
	std::vector<Texture> ResourceManager::LoadTextures(std::string const& sourcePath)
	{
		RESOURCEMANAGER_LOADTEXTURE;
		return std::vector<Texture>();
	}


	Texture ResourceManager::LoadTexture(std::string const& sourcePath)
	{
		RESOURCEMANAGER_LOADTEXTURE;
		return Chroma::TexureLoader::Load2DTexture(sourcePath);
	}


	std::vector<MeshData> ResourceManager::LoadModels(std::string const& sourcePath)
	{
		RESOURCEMANAGER_LOADMODEL;
		std::vector<MeshData> meshList;
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


	void ResourceManager::LoadModels(std::string const& sourcePath, std::vector<MeshData>* meshDatas)
	{
		RESOURCEMANAGER_LOADMODEL;
		std::vector<MeshData> meshList;
		ModelLoader::LoadThreadSafe(sourcePath, meshList);

		if (meshList.size() > 0)
			meshDatas = &meshList;

		else
			CHROMA_ERROR("RESOURCE MANAGER :: LoadModel :: Cannot find model at : {}", sourcePath);

	}


	MeshData ResourceManager::LoadModel(std::string const& sourcePath)
	{
		RESOURCEMANAGER_LOADMODEL;
		std::vector<MeshData> meshList;
		ModelLoader::LoadThreadSafe(sourcePath, meshList);
		if (meshList.size() > 0)
		{
			return meshList[0];
		}
		else
		{
			CHROMA_ERROR("RESOURCE MANAGER :: LoadModel :: Cannot find model at : {}", sourcePath);
			return MeshData();
		}
	}

	void ResourceManager::LoadModel(std::string const& sourcePath, MeshData* meshData)
	{
		RESOURCEMANAGER_LOADMODEL;
		std::string timer_timerName = "Model Load took : ";
		auto timer_start = std::chrono::high_resolution_clock::now();
		std::vector<MeshData> meshList;
		ModelLoader::LoadThreadSafe(sourcePath, meshList);

		if (meshList.size() > 0)
		{
			meshData = &meshList[0];
		}
		else
		{
			CHROMA_ERROR("RESOURCE MANAGER :: LoadModel :: Cannot find model at : {}", sourcePath);
		}

	}


	std::vector<Take> ResourceManager::LoadAnimations(std::string const& sourcePath)
	{
		RESOURCEMANAGER_LOADANIMATION
		return std::vector<Take>();
	}
	Take ResourceManager::LoadAnimation(std::string const& sourcePath)
	{
		RESOURCEMANAGER_LOADANIMATION
		return Take();
	}
}


