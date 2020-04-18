#include "SceneManager.h"


namespace Chroma
{
	void SceneManager::Init()
	{
	}

	void SceneManager::SaveScene(const char* sourceScenePath)
	{
		// Create New JSON
		JSONScene SceneJSON;

		// Serializing Entities
		for (std::pair<UID, IEntity*> const& uidentity : Chroma::Scene::GetAllEntities())
		{
			ISerializer* serializer = FactorySerializer::GetSerializer(Chroma::Type::Serialization::kJSON);
			uidentity.second->Serialize(serializer);
			SceneJSON.AddNewEntity(serializer);
		}

		// Serializing Components
		for (std::pair<UID, IComponent*> const& uidcomponent : Chroma::Scene::GetAllComponents())
		{
			ISerializer* serializer = FactorySerializer::GetSerializer(Chroma::Type::Serialization::kJSON);
			uidcomponent.second->Serialize(serializer);
			SceneJSON.AddNewComponent(serializer);
		}

		// Write JSON
		SceneJSON.Write(sourceScenePath);
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