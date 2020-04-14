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
			ISerializer* serializer = FactorySerializer::GetSerializer(Serialization::FORMAT::JSON);
			uidentity.second->Serialize(serializer);
			SerializeEntityToJson(serializer, SceneJSON);
		}

		// Serializing Components
		for (std::pair<UID, IComponent*> const& uidcomponent : Chroma::Scene::GetAllComponents())
		{
			ISerializer* serializer = FactorySerializer::GetSerializer(Serialization::FORMAT::JSON);
			uidcomponent.second->Serialize(serializer);
			SerializeComponentToJSON(serializer, SceneJSON);
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


	void SceneManager::SerializeComponentToJSON(ISerializer*& serialized, JSONScene& json)
	{
		json.AddNewComponent(serialized->m_UID);

		switch (serialized->m_ComponentType)
		{
		case(Chroma::Type::Component::kIComponent):
		{
			break;
		}
		case(Chroma::Type::Component::kMeshComponent):
		{
			break;
		}
		}
	}

	void SceneManager::SerializeEntityToJson(ISerializer*& serialized, JSONScene& json)
	{
		switch (serialized->m_EntityType)
		{
		case(Chroma::Type::Entity::kEntity):
		{
			json.AddNewEntity(serialized->m_UID);
			break;
		}
		case(Chroma::Type::Entity::kIEntity):
		{
			json.AddNewEntity(serialized->m_UID);
			break;
		}
		}
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