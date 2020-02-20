#include "SceneManager.h"


namespace Chroma
{
	void SceneManager::Init()
	{
	}

	void SceneManager::SaveScene(const char* sourceScenePath)
	{
		for (std::pair<UID, IComponent*> const& uidcomponent : Chroma::Scene::GetAllComponents())
		{
			ISerializer* serializer = FactorySerializer::GetSerializer(Serialization::FORMAT::JSON);
			uidcomponent.second->Serialize(serializer);
		}
	}

	void SceneManager::LoadScene(const char* destinationScenePath)
	{
	}
}