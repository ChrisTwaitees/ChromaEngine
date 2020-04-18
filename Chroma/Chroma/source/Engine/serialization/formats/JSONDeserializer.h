#ifndef CHROMA_JSON_DESERIALIZER_H
#define CHROMA_JSON_DESERIALIZER_H

#include <serialization/IDeserializer.h>
#include <serialization/formats/Json.h>

class JSONDeserializer : public IDeserializer
{
public:

	template <class objectType, typename ChromaType>
	objectType CreateObject(ChromaType type, const rapidjson::Value& jsonData)
	{
		CHROMA_WARN("Could not create Object Based on Deserialization Data!");
	}

	// Entity
	template<>
	Entity* CreateObject<Entity*, Chroma::Type::Component>(Chroma::Type::Component type, const rapidjson::Value& jsonData)
	{
		DESERIALIZE_START
		// create new component pointer
		Entity* newIEntity = new Entity;

		// Setting Members
		for (rapidjson::Value::ConstMemberIterator entityValue = jsonData.MemberBegin(); entityValue != jsonData.MemberEnd(); ++entityValue)
		{
			CHROMA_INFO("Entity attr : {0}", entityValue->name.GetString());
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