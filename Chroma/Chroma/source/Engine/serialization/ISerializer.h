#ifndef _CHROMA_SERIALIZER_H_
#define _CHROMA_SERIALIZER_H_

// Factory method read from : https://realpython.com/factory-method-python/
#include <common/PrecompiledHeader.h>
#include <editor/EditorProperty.h>
#include <uid/UID.h>

struct ISerializerKey
{
public:
	const char* m_Name{ "" };
	EditorProperty m_EditorProperty{ Chroma::Type::EditorProperty::kNullEditorProperty };
	friend bool operator< (const ISerializerKey& lhs, const ISerializerKey& rhs) {	return true; };
};


class ISerializer
{
public:
	virtual void StartObject(const Chroma::Type::Component& componentType, const UID& uid) = 0;
	virtual void StartObject(const Chroma::Type::Entity& entityType, const UID& uid) = 0;

	// TYPES
	Chroma::Type::Component m_ComponentType{ Chroma::Type::Component::kIComponent };
	Chroma::Type::Entity m_EntityType{ Chroma::Type::Entity::kIEntity };

	// UID
	UID m_UID;

	// PROPERTIES
	template<typename PropertyType>
	void AddProperty(const char* key, PropertyType value, EditorProperty editorPrpty = EditorProperty())
	{
		CHROMA_WARN("Serialization Type Not Supported for key : {0}", key);
	}

	template<>
	void AddProperty<std::string*>(const char* key, std::string* value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_StringProperties.emplace(std::make_pair(serializeKey, value));
	}

	template<>
	void AddProperty<const char*>(const char* key, const char* value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_CharProperties.emplace(std::make_pair(serializeKey, value));
	}

	template<>
	void AddProperty<float*>(const char* key, float* value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_FloatProperties.emplace(std::make_pair(serializeKey, value));
	}

	template<>
	void AddProperty<double*>(const char* key, double* value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_DoubleProperties.emplace(std::make_pair(serializeKey, value));
	}

	template<>
	void AddProperty<int*>(const char* key, int* value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_IntProperties.emplace(std::make_pair(serializeKey, value));
	}

	template<>
	void AddProperty<unsigned int*>(const char* key, unsigned int* value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_UIntProperties.emplace(std::make_pair(serializeKey, value));
	}

	template<>
	void AddProperty<glm::vec2*>(const char* key, glm::vec2* value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_Vec2Properties.emplace(std::make_pair(serializeKey, value));
	}

	template<>
	void AddProperty<glm::vec3*>(const char* key, glm::vec3* value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_Vec3Properties.emplace(std::make_pair(serializeKey, value));
	}

	template<>
	void AddProperty<glm::vec4*>(const char* key, glm::vec4* value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_Vec4Properties.emplace(std::make_pair(serializeKey, value));
	}

	template<>
	void AddProperty<glm::quat*>(const char* key, glm::quat* value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_QuatProperties.emplace(std::make_pair(serializeKey, value));
	}

	template<>
	void AddProperty<glm::mat3*>(const char* key, glm::mat3* value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_Mat3Properties.emplace(std::make_pair(serializeKey, value));
	}

	template<>
	void AddProperty<glm::mat4*>(const char* key, glm::mat4* value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_Mat4Properties.emplace(std::make_pair(serializeKey, value));
	}

	template<>
	void AddProperty<std::vector<UID>>(const char* key, std::vector<UID> value, EditorProperty editorPrpty)
	{
		ISerializerKey serializeKey;
		serializeKey.m_Name = key;
		serializeKey.m_EditorProperty = editorPrpty;
		m_UIDVectors.emplace(std::make_pair(serializeKey, value));
	}


	std::map<ISerializerKey, float*> m_FloatProperties;
	std::map<ISerializerKey, glm::vec2*> m_Vec2Properties;
	std::map<ISerializerKey, glm::vec3*> m_Vec3Properties;
	std::map<ISerializerKey, glm::vec4*> m_Vec4Properties;

	std::map<ISerializerKey, const char*> m_CharProperties;
	std::map<ISerializerKey, std::string*> m_StringProperties;

	std::map<ISerializerKey, int*> m_IntProperties;
	std::map<ISerializerKey, unsigned int*> m_UIntProperties;

	std::map<ISerializerKey, double*> m_DoubleProperties;

	std::map<ISerializerKey, glm::quat*> m_QuatProperties;
	std::map<ISerializerKey, glm::mat3*> m_Mat3Properties;
	std::map<ISerializerKey, glm::mat4*> m_Mat4Properties;

	std::map<ISerializerKey, std::vector<UID>> m_UIDVectors;

	ISerializer() {};
	virtual ~ISerializer() {};
private:

};

#endif 


