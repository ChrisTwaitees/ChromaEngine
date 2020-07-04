#ifndef CHROMA_JSON_DESERIALIZER_H
#define CHROMA_JSON_DESERIALIZER_H

#include <serialization/IDeserializer.h>
#include <serialization/formats/Json.h>
#include <serialization/scene/JSONScene.h>


namespace Chroma
{
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
							newIEntity->AddMeshComponentUID(UID(componentUID->GetInt()));
							break;
						}
						case(Type::kSkinnedMeshComponent):
						{
							newIEntity->AddMeshComponentUID(UID(componentUID->GetInt()));
							break;
						}
						case(Type::kModelComponent):
						{
							newIEntity->AddMeshComponentUID(UID(componentUID->GetInt()));
							break;
						}
						case(Type::kPhysicsComponent):
						{
							newIEntity->AddPhysicsComponentUID(UID(componentUID->GetInt()));
							break;
						}
						case(Type::kCharacterControllerComponent):
						{
							newIEntity->AddCharacterControllerComponentUID(UID(componentUID->GetInt()));
							break;
						}
						case(Type::kAnimationComponent):
						{
							newIEntity->AddAnimationComponentUID(UID(componentUID->GetInt()));
							break;
						}
						case(Type::kAnimationConstraintComponent):
						{
							newIEntity->AddAnimConstraintComponentUID(UID(componentUID->GetInt()));
							break;
						}
						case(Type::kStateMachineComponent):
						{
							newIEntity->AddStateMachineComponentUID(UID(componentUID->GetInt()));
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
				std::string componentAttrKey(componentValue->name.GetString());
				if (componentAttrKey == "m_ParentEntityUID")
				{
					newStaticMesh->SetParentEntityUID(UID(componentValue->value.GetString()));
				}
				else if (componentAttrKey == "m_SourcePath")
				{
					newStaticMesh->SetSourcePath(componentValue->value.GetString());
				}
				else if (componentAttrKey == "m_Rotation")
				{
					glm::quat newRotation;
					newRotation.x = componentValue->value.GetArray()[0].GetFloat();
					newRotation.y = componentValue->value.GetArray()[1].GetFloat();
					newRotation.z = componentValue->value.GetArray()[2].GetFloat();
					newRotation.w = componentValue->value.GetArray()[3].GetFloat();
					newStaticMesh->SetRotation(newRotation);
				}
				else if (componentAttrKey == "m_Translation")
				{
					glm::vec3 newTranslation;
					newTranslation.x = componentValue->value.GetArray()[0].GetFloat();
					newTranslation.y = componentValue->value.GetArray()[1].GetFloat();
					newTranslation.z = componentValue->value.GetArray()[2].GetFloat();
					newStaticMesh->SetTranslation(newTranslation);
				}
				else if (componentAttrKey == "m_Scale")
				{
					glm::vec3 newScale;
					newScale.x = componentValue->value.GetArray()[0].GetFloat();
					newScale.y = componentValue->value.GetArray()[1].GetFloat();
					newScale.z = componentValue->value.GetArray()[2].GetFloat();
					newStaticMesh->SetScale(newScale);
				}
				else if (componentAttrKey == CHROMA_MATERIAL_KEY)
				{
					DeserializeMaterial(newStaticMesh, componentValue->value);
				}
				else
				{
					CHROMA_WARN("{} Serialized Attribute Not Accounted for!", componentValue->name.GetString());
				}
			}

			// Rebuild Mesh
			newStaticMesh->RebuildMesh();

			// Return
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

		// Light
		template<>
		Light* CreateObject<Light*, Chroma::Type::Component>(Chroma::Type::Component type, const rapidjson::Value& jsonData)
		{
			DESERIALIZE_START
				// create new component pointer
			Light* newLight = new Light;

			// Setting Members
			for (rapidjson::Value::ConstMemberIterator lightValue = jsonData.MemberBegin(); lightValue != jsonData.MemberEnd(); ++lightValue)
			{
				CHROMA_INFO("Light attr : {0}", lightValue->name.GetString());
				std::string lightValueName = lightValue->name.GetString();

				// transforms
				if (lightValueName == "m_Position")
				{
					glm::vec3 newTranslation(0.0);
					newTranslation.x = lightValue->value.GetArray()[0].GetFloat();
					newTranslation.y = lightValue->value.GetArray()[1].GetFloat();
					newTranslation.z = lightValue->value.GetArray()[2].GetFloat();
					newLight->SetTranslation(newTranslation);
				}

				// direction
				if (lightValueName == "m_Direction") 
				{
					glm::vec3 newDirection(0.0);
					newDirection.x = lightValue->value.GetArray()[0].GetFloat();
					newDirection.y = lightValue->value.GetArray()[1].GetFloat();
					newDirection.z = lightValue->value.GetArray()[2].GetFloat();
					newLight->SetDirection(newDirection);
				}

				// color
				if (lightValueName == "m_Diffuse")
				{
					glm::vec3 newDiffuse(0.0);
					newDiffuse.x = lightValue->value.GetArray()[0].GetFloat();
					newDiffuse.y = lightValue->value.GetArray()[1].GetFloat();
					newDiffuse.z = lightValue->value.GetArray()[2].GetFloat();
					newLight->SetDiffuse(newDiffuse);
				}

				// falloffs
				if (lightValueName == "m_Intensity")
					newLight->SetIntensity(lightValue->value.GetFloat());

				if (lightValueName == "m_Linear") 
					newLight->setLinear(lightValue->value.GetFloat());

				if (lightValueName == "m_Quadratic")
					newLight->setQuadratic(lightValue->value.GetFloat());

				if (lightValueName == "m_Constant")
					newLight->setConstant(lightValue->value.GetFloat());

				if (lightValueName == "m_Type")
					newLight->SetLightType(Chroma::Type::GetType<Chroma::Type::Light>(lightValue->value.GetString()));

			}
			// UID is set after creation
			return newLight;
		}


		JSONDeserializer() {};
		~JSONDeserializer() {};

	private:

		// Materials 

			// add
		template<typename meshComponent>
		void DeserializeMaterial(meshComponent mesh, const rapidjson::Value& jsonData) {
			CHROMA_FATAL("MeshComponent Type Not Supported For Material Deserialization!");
		};

		template<>
		void DeserializeMaterial<StaticMesh*>(StaticMesh* meshCmp, const rapidjson::Value& jsonData) {
			CHROMA_INFO("Deserializing Material");
			for (rapidjson::Value::ConstMemberIterator materialValue = jsonData.MemberBegin(); materialValue != jsonData.MemberEnd(); ++materialValue)
			{
				std::string materialAttrKey(materialValue->name.GetString());
				CHROMA_INFO("Material attr : {0}", materialAttrKey);

				// Textures
				if (materialAttrKey == CHROMA_MATERIAL_TEXTURES_KEY)
				{
					for (rapidjson::Value::ConstMemberIterator textureValue = materialValue->value.MemberBegin(); textureValue != materialValue->value.MemberEnd(); ++textureValue)
					{
						// Create Texture and set type
						Texture newTexture(textureValue->value.GetString());
						newTexture.SetType(Chroma::Type::GetType<Chroma::Type::Texture>(textureValue->name.GetString()));
						// Add Texture
						meshCmp->AddTexture(newTexture);
					}
				}
			}
		};

	};
}

#endif // CHROMA_JSON_DESERIALIZER_H