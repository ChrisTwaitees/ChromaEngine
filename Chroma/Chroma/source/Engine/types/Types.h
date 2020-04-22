
#ifndef CHROMA_TYPES_H
#define CHROMA_TYPES_H
#include <string>
#include <map>
#include <logging/Log.h>

namespace Chroma
{
	class Type
	{
	public:
		enum Component
		{
			kNullComponent = -99,
			kIComponent = 0,
			kStateMachineComponent = 1,
			kAnimationComponent = 2,
			kAnimationConstraintComponent = 3,
			kIKAnimationConstraintComponent = 4,
			kCharacterControllerComponent = 5,
			kCharacterPhysicsComponent = 6,
			kPhysicsComponent = 7,
			kMeshComponent = 8,
			kUIComponent = 9,
			kLightComponent = 10,
			kModelComponent = 11,
			kStaticMeshComponent = 12,
			kSkinnedMeshComponent = 13
		};

		enum Entity {
			kNullEntity = -99,
			kIEntity = 0,
			kEntity = 1
		};

		enum Serialization {
			kNullSerialization = -99,
			kJSON = 0
		};

		enum DataType {
			kNull = 0
		};

		enum EditorProperty {
			kNullEditorProperty = 0,
			kSourcePathProperty = 1, 
			kFloatPropery = 2, 
			kIntProperty = 3, 
			kStringProperty = 4,
			kTransformProperty = 5,
			kColorProperty = 6,
			kMaterialProperty = 7,
			kMaterialTextureProperty = 8,
			kMaterialUniformProperty = 9,
			kMaterialUniformColorProperty = 10
		};

		// Getters
		// Name
		static std::string GetName(Component componentEnum);
		static std::string GetName(Entity entityEnum);
		static std::string GetName(Serialization serializedType);
		static std::string GetName(DataType dataType);
		static std::string GetName(EditorProperty editorPropertyType);

		// Type
		template<typename T>
		static T GetType(const std::string& name)
		{
			CHROMA_WARN("Chroma Type : {}, NOT Supported!", name);
		};
		// components
		template<>
		static Type::Component GetType<Type::Component>(const std::string& objectTypeName)
		{
			if (m_ComponentMap.find(objectTypeName) != m_ComponentMap.end())
				return m_ComponentMap.find(objectTypeName)->second;
			else
				return Type::Component::kNullComponent;
		}
		// entities
		template<>
		static Type::Entity GetType<Type::Entity>(const std::string& objectTypeName)
		{
			if (m_EntityMap.find(objectTypeName) != m_EntityMap.end())
				return m_EntityMap.find(objectTypeName)->second;
			else
				return Type::Entity::kNullEntity;
		}
		// serialization
		template<>
		static Type::Serialization GetType<Type::Serialization>(const std::string& objectTypeName)
		{
			if (m_SerializationMap.find(objectTypeName) != m_SerializationMap.end())
				return m_SerializationMap.find(objectTypeName)->second;
			else
				return Type::Serialization::kNullSerialization;
		}

	private:
		// Type string maps
		static std::map<std::string, Type::Component> m_ComponentMap;
		static std::map<std::string, Type::Entity> m_EntityMap;
		static std::map<std::string, Type::Serialization> m_SerializationMap;
		static std::map<std::string, Type::DataType> m_DataTypeMap;
		static std::map<std::string, Type::EditorProperty> m_EditorPropertiesMap;
	};

}




#endif //CHROMA_TYPES_H
