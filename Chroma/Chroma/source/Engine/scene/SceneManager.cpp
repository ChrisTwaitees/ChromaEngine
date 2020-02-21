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
		CHROMA_INFO("SCENE MANAGER :: Loading Scene from : {0}", destinationScenePath);
		ClearScene();
	}
	void SceneManager::ClearScene()
	{
		// Destroy Components
		for (std::pair<UID, IComponent*> const& uidcomponent : Chroma::Scene::GetAllComponents())
		{
			uidcomponent.second->Destroy();
			Chroma::Scene::RemoveComponent(uidcomponent.first);
		}
	}
}