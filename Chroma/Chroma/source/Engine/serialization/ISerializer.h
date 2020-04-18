#ifndef _CHROMA_SERIALIZER_H_
#define _CHROMA_SERIALIZER_H_

// Factory method read from : https://realpython.com/factory-method-python/
#include <common/PrecompiledHeader.h>
#include <types/Types.h> 
#include <uid/UID.h>



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
	void AddProperty(const char* key, PropertyType value)
	{
		CHROMA_WARN("Serialization Type Not Supported for key : {0}", key);
	}

	template<>
	void AddProperty<std::string*>(const char* key, std::string* value)
	{
		m_StringProperties.emplace(std::make_pair(key, value));
	}

	template<>
	void AddProperty<const char*>(const char* key, const char* value)
	{
		m_CharProperties.emplace(std::make_pair(key, value));
	}

	template<>
	void AddProperty<float*>(const char* key, float* value)
	{
		m_FloatProperties.emplace(std::make_pair(key, value));
	}

	template<>
	void AddProperty<double*>(const char* key, double* value)
	{
		m_DoubleProperties.emplace(std::make_pair(key, value));
	}

	template<>
	void AddProperty<int*>(const char* key, int* value)
	{
		m_IntProperties.emplace(std::make_pair(key, value));
	}

	template<>
	void AddProperty<unsigned int*>(const char* key, unsigned int* value)
	{
		m_UIntProperties.emplace(std::make_pair(key, value));
	}

	template<>
	void AddProperty<glm::vec2*>(const char* key, glm::vec2* value)
	{
		m_Vec2Properties.emplace(std::make_pair(key, value));
	}

	template<>
	void AddProperty<glm::vec3*>(const char* key, glm::vec3* value)
	{
		m_Vec3Properties.emplace(std::make_pair(key, value));
	}

	template<>
	void AddProperty<glm::vec4*>(const char* key, glm::vec4* value)
	{
		m_Vec4Properties.emplace(std::make_pair(key, value));
	}

	template<>
	void AddProperty<glm::quat*>(const char* key, glm::quat* value)
	{
		m_QuatProperties.emplace(std::make_pair(key, value));
	}

	template<>
	void AddProperty<glm::mat3*>(const char* key, glm::mat3* value)
	{
		m_Mat3Properties.emplace(std::make_pair(key, value));
	}

	template<>
	void AddProperty<glm::mat4*>(const char* key, glm::mat4* value)
	{
		m_Mat4Properties.emplace(std::make_pair(key, value));
	}

	virtual const char* ToString() = 0;


	std::map<const char*, float*> m_FloatProperties;
	std::map<const char*, glm::vec3*> m_Vec3Properties;

	std::map<const char*, const char*> m_CharProperties;
	std::map<const char*, std::string*> m_StringProperties;

	std::map<const char*, int*> m_IntProperties;
	std::map<const char*, unsigned int*> m_UIntProperties;

	std::map<const char*, double*> m_DoubleProperties;

	std::map<const char*, glm::vec2*> m_Vec2Properties;
	std::map<const char*, glm::vec4*> m_Vec4Properties;
	std::map<const char*, glm::quat*> m_QuatProperties;
	std::map<const char*, glm::mat3*> m_Mat3Properties;
	std::map<const char*, glm::mat4*> m_Mat4Properties;

	ISerializer() {};
	virtual ~ISerializer() {};
private:

};

#endif 


