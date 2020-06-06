#include "ResourceManager.h"


namespace Chroma
{
	std::vector<std::future<void>> ResourceManager::m_Futures;
	std::mutex ResourceManager::m_Mutex;

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

	MeshData ResourceManager::LoadModel(std::string const& sourcePath)
	{
		RESOURCEMANAGER_LOADMODEL;
		std::vector<MeshData> meshList;
		ModelLoader::LoadThreadSafe(sourcePath, meshList);

		if (meshList.size() > 0)
		{
			ModelLoader::CalculateBBoxCentroid(meshList[0]);
			return meshList[0];
		}
		else
		{
			CHROMA_ERROR("RESOURCE MANAGER :: LoadModel :: Cannot find model at : {}", sourcePath);
			return MeshData();
		}
	}

	std::vector<MeshData> ResourceManager::LoadModels(std::string const& sourcePath)
	{
		RESOURCEMANAGER_LOADMODEL;
		std::vector<MeshData> meshList;
		ModelLoader::LoadThreadSafe(sourcePath, meshList);

		if (meshList.size() > 0)
		{
			for (MeshData& meshData : meshList)
			{
				ModelLoader::CalculateBBoxCentroid(meshData);
			}
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
		// lock resources from accessing to prevent read/write conflicts
		std::lock_guard<std::mutex> lock(m_Mutex);
		// send asynchronous job, storing in futures
		m_Futures.push_back(std::async(std::launch::async, LoadModelsAsync, sourcePath, meshDatas));
	}

	void ResourceManager::LoadModel(std::string const& sourcePath, MeshData* meshData)
	{
		// lock resources from accessing to prevent read/write conflicts
		std::lock_guard<std::mutex> lock(m_Mutex);
		// send asynchronous job, storing in futures
		m_Futures.push_back(std::async(std::launch::async, LoadModelAsync, sourcePath, meshData));
	}

	void ResourceManager::LoadModelAsync(const std::string& sourcePath, MeshData* meshdata)
	{
		RESOURCEMANAGER_LOADMODEL;
		std::vector<MeshData> meshList;
		ModelLoader::LoadThreadSafe(sourcePath, meshList);

		if (meshList.size() > 0)
		{
			*meshdata = meshList[0];
		}
		else
		{
			CHROMA_ERROR("RESOURCE MANAGER :: LoadModel :: Cannot find model at : {}", sourcePath);
		}
	}

	void ResourceManager::LoadModelsAsync(const std::string& sourcePath, std::vector<MeshData>* meshdatas)
	{
		RESOURCEMANAGER_LOADMODEL;
		ModelLoader::LoadThreadSafe(sourcePath, *meshdatas);
		if (meshdatas->size() == 0)

			CHROMA_ERROR("RESOURCE MANAGER :: LoadModel :: Cannot find model at : {}", sourcePath);
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


