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

	Texture ResourceManager::Load2DTexture(std::string const& sourcePath)
	{
		RESOURCEMANAGER_LOADTEXTURE;
		return TexureLoader::Load2DTexture(sourcePath);
	}

	void ResourceManager::Load2DTexture(const std::string& sourcePath, TextureData* textureData)
	{
		textureData->sourcePath = sourcePath;
		// lock resources from accessing to prevent read/write conflicts
		std::unique_lock<std::mutex> lock(m_Mutex);
		// send asynchronous job, storing in futures
		m_Futures.push_back(std::async(std::launch::async, Load2DTextureAsync, sourcePath, textureData));
		lock.unlock();
	}

	void ResourceManager::LoadHDRTexture(const std::string& sourcePath, TextureData* textureData)
	{
		textureData->sourcePath = sourcePath;
		// lock resources from accessing to prevent read/write conflicts
		std::unique_lock<std::mutex> lock(m_Mutex);
		// send asynchronous job, storing in futures
		m_Futures.push_back(std::async(std::launch::async, LoadHDRTextureAsync, sourcePath, textureData));
		lock.unlock();
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

	void ResourceManager::Load2DTextureAsync(const std::string& sourcePath, TextureData* textureData)
	{
		RESOURCEMANAGER_LOADTEXTURE;
		TextureData newTexData;

		TexureLoader::Load2DTextureDataThreadSafe(sourcePath, newTexData);
		newTexData.sourcePath = sourcePath;

		if (!newTexData.imageData)
			CHROMA_ERROR("RESOURCE MANAGER :: Load2DTexture :: Cannot find texture at : {}", sourcePath);
		else
			*textureData = newTexData;
	}

	void ResourceManager::LoadHDRTextureAsync(const std::string& sourcePath, TextureData* textureData)
	{
		RESOURCEMANAGER_LOADTEXTURE;
		TextureData newTexData;

		TexureLoader::LoadHDRTextureDataThreadSafe(sourcePath, newTexData);
		newTexData.sourcePath = sourcePath;

		if (!newTexData.imageData)
			CHROMA_ERROR("RESOURCE MANAGER :: LoadHDRTexture :: Cannot find texture at : {}", sourcePath);
		else
			*textureData = newTexData;
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


