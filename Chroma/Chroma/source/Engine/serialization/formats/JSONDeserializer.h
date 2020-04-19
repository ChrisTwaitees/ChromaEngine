#ifndef CHROMA_JSON_DESERIALIZER_H
#define CHROMA_JSON_DESERIALIZER_H

#include <serialization/IDeserializer.h>
#include <serialization/formats/Json.h>

using namespace Chroma;

class JSONDeserializer : public IDeserializer
{
public:

	template <class objectType, typename ChromaType>
	objectType CreateObject(ChromaType type, const rapidjson::Value& jsonData)
	{
		CHROMA_WARN("Could not create Object Based on Deserialization Data!");
		return nullptr;
	}

	// Entity
	template<>
	Entity* CreateObject<Entity*, Chroma::Type::Entity>(Chroma::Type::Entity type, const rapidjson::Value& jsonData)
	{
		DESERIALIZE_START
		// create new component pointer
		Entity* newIEntity = new Entity;

		// Setting Members
		for (rapidjson::Value::ConstMemberIterator entityValue = jsonData.MemberBegin(); entityValue != jsonData.MemberEnd(); ++entityValue)
		{
			// Transform Attrs
			CHROMA_INFO("Entity attr : {0}", entityValue->name.GetString());
			std::string entityValueName = entityValue->name.GetString();

			// Setting Members
			if (entityValueName == "m_Name")
				newIEntity->SetName(entityValue->value.GetString());

			// transforms
			if (entityValueName == "m_Translation") // translation
			{
				glm::vec3 newTranslation(0.0);
				newTranslation.x = entityValue->value.GetArray()[0].GetFloat();
				newTranslation.y = entityValue->value.GetArray()[1].GetFloat();
				newTranslation.z = entityValue->value.GetArray()[2].GetFloat();
				newIEntity->SetTranslation(newTranslation);
			}
			if (entityValueName == "m_Scale") // scale
			{
				glm::vec3 newScale(0.0);
				newScale.x = entityValue->value.GetArray()[0].GetFloat();
				newScale.y = entityValue->value.GetArray()[1].GetFloat();
				newScale.z = entityValue->value.GetArray()[2].GetFloat();
				newIEntity->SetScale(newScale);
			}
			if (entityValueName == "m_Rotation") // rotation
			{
				glm::quat newRotation = glm::quat();
				newRotation.x = entityValue->value.GetArray()[0].GetFloat();
				newRotation.y = entityValue->value.GetArray()[1].GetFloat();
				newRotation.z = entityValue->value.GetArray()[2].GetFloat();
				newRotation.w = entityValue->value.GetArray()[3].GetFloat();
				newIEntity->SetRotation(newRotation);
			}
			
			// component UIDs
			if (Type::GetType<Type::Component>(entityValueName) != Type::Component::kNullComponent)
			{
				for (rapidjson::Value::ConstValueIterator componentUID = entityValue->value.Begin(); componentUID != entityValue->value.End(); ++componentUID)
				{
					switch (Type::GetType<Type::Component>(entityValueName))
					{
					case(Type::kMeshComponent):
					{
						newIEntity->AddMeshComponentUID(UID(componentUID->GetString()));
						break;
					}
					case(Type::kSkinnedMeshComponent):
					{
						newIEntity->AddMeshComponentUID(UID(componentUID->GetString()));
						break;
					}
					case(Type::kModelComponent):
					{
						newIEntity->AddMeshComponentUID(UID(componentUID->GetString()));
						break;
					}
					case(Type::kPhysicsComponent):
					{
						newIEntity->AddPhysicsComponentUID(UID(componentUID->GetString()));
						break;
					}
					case(Type::kCharacterControllerComponent):
					{
						newIEntity->AddCharacterControllerComponentUID(UID(componentUID->GetString()));
						break;
					}
					case(Type::kAnimationComponent):
					{
						newIEntity->AddAnimationComponentUID(UID(componentUID->GetString()));
						break;
					}
					case(Type::kAnimationConstraintComponent):
					{
						newIEntity->AddAnimConstraintComponentUID(UID(componentUID->GetString()));
						break;
					}
					case(Type::kStateMachineComponent):
					{
						newIEntity->AddStateMachineComponentUID(UID(componentUID->GetString()));
						break;
					}
					default :
					{
						CHROMA_WARN("Unsupported ChromaComponent to add to Entity {0}", entityValueName);
					}
					}
				}

			}

		}
		return newIEntity;
	}


	// StaticMesh
	template<>
	StaticMesh* CreateObject<StaticMesh*, Chroma::Type::Component>(Chroma::Type::Component type, const rapidjson::Value& jsonData)
	{
		DESERIALIZE_START
		// create new component pointer
		StaticMesh* newStaticMesh = new StaticMesh;

		// Setting Members
		for (rapidjson::Value::ConstMemberIterator componentValue = jsonData.MemberBegin(); componentValue != jsonData.MemberEnd(); ++componentValue)
		{
			CHROMA_INFO("Component attr : {0}", componentValue->name.GetString());
		}
		return newStaticMesh;
	}

	// SkinnedMesh
	template<>
	SkinnedMesh* CreateObject<SkinnedMesh*, Chroma::Type::Component>(Chroma::Type::Component type, const rapidjson::Value& jsonData)
	{
		DESERIALIZE_START
		// create new component pointer
		SkinnedMesh* newSkinnedMesh = new SkinnedMesh;

		// Setting Members
		for (rapidjson::Value::ConstMemberIterator componentValue = jsonData.MemberBegin(); componentValue != jsonData.MemberEnd(); ++componentValue)
		{
			CHROMA_INFO("Component attr : {0}", componentValue->name.GetString());
		}

		// UID is set after creation
		return newSkinnedMesh;
	}


	JSONDeserializer() {};
	~JSONDeserializer() {};
};

#endif // CHROMA_JSON_DESERIALIZER_H