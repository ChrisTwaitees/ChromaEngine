#include "SceneManager.h"
#include <scene/Scene.h>
#include <serialization/scene/JSONScene.h>
#include <serialization/formats/JSONDeserializer.h>

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

		// Serialize Scene Entities
		SceneJSON.AddIBL(Chroma::Scene::GetIBL());

		// Write JSON
		SceneJSON.Write(sourceScenePath);
	}

	void SceneManager::LoadScene(const char* sourcepath)
	{
		CHROMA_INFO("SCENE MANAGER :: Loading Scene from : {0}", sourcepath);

		// Clear Scene
		ClearScene();

		// Load Scene JSON
		JSONScene jsonScene(sourcepath);

		// Start Scene Build
		Chroma::Scene::PreSceneBuild();

		// Components
		rapidjson::Value& componentsRoot = jsonScene.GetComponents();
		for (rapidjson::Value::ConstMemberIterator componentTypes = componentsRoot.MemberBegin(); componentTypes != componentsRoot.MemberEnd(); ++componentTypes)
		{
			// Get Deserializer
			JSONDeserializer* deserializer = new JSONDeserializer;

			// Create based on m_Type
			switch (Type::GetType<Type::Component>(componentTypes->name.GetString()))
			{
			case(Type::Component::kStaticMeshComponent) :
			{
				for (rapidjson::Value::ConstMemberIterator staticMeshComponent = componentTypes->value.MemberBegin(); staticMeshComponent != componentTypes->value.MemberEnd(); ++staticMeshComponent)
				{
					StaticMesh* newStaticMeshComponent = deserializer->CreateObject<StaticMesh*>(Chroma::Type::Component::kStaticMeshComponent, staticMeshComponent->value);
					newStaticMeshComponent->SetUID(UID(staticMeshComponent->name.GetString()));
					//Chroma::Scene::AddMeshComponent(newStaticMeshComponent);
				}
				break;
			}
			case(Type::Component::kSkinnedMeshComponent):
			{
				for (rapidjson::Value::ConstMemberIterator skinnedMeshComponent = componentTypes->value.MemberBegin(); skinnedMeshComponent != componentTypes->value.MemberEnd(); ++skinnedMeshComponent)
				{
					SkinnedMesh* newSkinnedMeshComponent = deserializer->CreateObject<SkinnedMesh*>(Chroma::Type::Component::kSkinnedMeshComponent, skinnedMeshComponent->value);
					newSkinnedMeshComponent->SetUID(UID(skinnedMeshComponent->name.GetString()));
					//Chroma::Scene::AddMeshComponent(newSkinnedMeshComponent);
				}
				break;
			}
			case(Type::Component::kLightComponent):
			{
				for (rapidjson::Value::ConstMemberIterator lightComponent = componentTypes->value.MemberBegin(); lightComponent != componentTypes->value.MemberEnd(); ++lightComponent)
				{
					Light* newLight = deserializer->CreateObject<Light*>(Chroma::Type::Component::kLightComponent, lightComponent->value);
					newLight->SetUID(UID(lightComponent->name.GetString()));
					Chroma::Scene::AddLight(newLight);
				}
				break;
			}
			};
		}
		
		// Entities
		rapidjson::Value& entitiesRoot = jsonScene.GetEntities();
		for (rapidjson::Value::ConstMemberIterator entityTypes = entitiesRoot.MemberBegin(); entityTypes != entitiesRoot.MemberEnd(); ++entityTypes)
		{
			// Get Deserializer
			JSONDeserializer* deserializer = new JSONDeserializer;

			// Create based on m_Type
			switch (Type::GetType<Type::Entity>(entityTypes->name.GetString()))
			{
			case(Type::Entity::kIEntity || Type::Entity::kEntity):
			{
				for (rapidjson::Value::ConstMemberIterator entity = entityTypes->value.MemberBegin(); entity != entityTypes->value.MemberEnd(); ++entity)
				{
					Entity* newEntity = deserializer->CreateObject<Entity*>(Chroma::Type::Entity::kEntity, entity->value);
					newEntity->SetUID(UID(entity->name.GetString()));
					Chroma::Scene::AddEntity(newEntity);
				}
			}
			};
		}
	
		// Scene Entities
		// IBL
		rapidjson::Value& ibl = jsonScene.GetIBL();
		std::string iblSourcePath = ibl.GetString();
		Chroma::Scene::GetIBL()->LoadIBL(iblSourcePath);
		
		// Init all entities
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