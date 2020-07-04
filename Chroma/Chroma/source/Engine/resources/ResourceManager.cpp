#include "ResourceManager.h"


namespace Chroma
{
	std::vector<std::future<void>> ResourceManager::m_Futures;
	std::mutex ResourceManager::m_Mutex;

	void ResourceManager::Init()
	{
		// Init Loaders
		Chroma::TextureLoader::Init();
		Chroma::ModelLoader::Init();

		// log
		CHROMA_INFO("Resource Manager Initialized.");
	}

	void ResourceManager::Update()
	{
		Chroma::TextureLoader::Update();
	}



	Texture ResourceManager::Create2DTexture(std::string const& sourcePath)
	{
		RESOURCEMANAGER_LOADTEXTURE;
		return TextureLoader::Create2DTexture(sourcePath);
	}

	void ResourceManager::Load2DTexture(const std::string& sourcePath, std::shared_ptr<TextureData> textureData)
	{
		// send asynchronous job, storing in futures
		m_Futures.push_back(std::async(std::launch::async, TextureLoader::Create2DTextureThreadSafe, sourcePath, textureData));
	}

	void ResourceManager::Load2DTexture(const std::string& sourcePath, TextureData& textureData)
	{
		// send asynchronous job, storing in futures
	//	m_Futures.push_back(std::async(std::launch::async, TextureLoader::Create2DTextureThreadSafe, sourcePath, std::ref(textureData)));
	}

	void ResourceManager::LoadHDRTexture(const std::string& sourcePath, TextureData& textureData)
	{
		// send asynchronous job, storing in futures
		//m_Futures.push_back(std::async(std::launch::async, TextureLoader::CreateHDRTextureThreadSafe, sourcePath, std::ref(textureData)));
	}

	void ResourceManager::LoadHDRTexture(const std::string& sourcePath, std::shared_ptr<TextureData> textureData)
	{
		m_Futures.push_back(std::async(std::launch::async, TextureLoader::CreateHDRTextureThreadSafe, sourcePath, textureData));
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
				ModelLoader::CalculateBBoxCentroid(meshData);

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
		// send asynchronous job, storing in futures
		m_Futures.push_back(std::async(std::launch::async, LoadModelsAsync, sourcePath, meshDatas));
	}

	void ResourceManager::LoadModel(std::string const& sourcePath, MeshData* meshData)
	{
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

	void ResourceManager::Test(std::string test, std::shared_ptr<TextureData> textureData)
	{
		textureData->sourcePath = test;
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


