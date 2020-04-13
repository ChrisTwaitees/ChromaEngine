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
			// TODO implement unique ptr for scope memory management
			ISerializer* serializer = FactorySerializer::GetSerializer(Serialization::FORMAT::JSON);
			uidcomponent.second->Serialize(serializer);
		}
	}

	void SceneManager::LoadScene(const char* destinationScenePath)
	{
		CHROMA_INFO("SCENE MANAGER :: Loading Scene from : {0}", destinationScenePath);
		ClearScene();
		Chroma::Scene::PreSceneBuild();



		Chroma::Scene::PostSceneBuild();
	}


	void SceneManager::ClearScene()
	{
		// Remove and Destroy Entities
		for (std::pair<UID, IEntity*> const& uidentity : Chroma::Scene::GetAllEntities())
		{
			uidentity.second->Destroy();
			Chroma::Scene::RemoveEntity(uidentity.first);
		}

		// Remove and Destroy Components
		for (std::pair<UID, IComponent*> const& uidcomponent : Chroma::Scene::GetAllComponents())
		{
			uidcomponent.second->Destroy();
			Chroma::Scene::RemoveComponent(uidcomponent.first);
		}

	}
}