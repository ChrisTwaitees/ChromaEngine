#ifndef CHROMA_JSON_SCENE_H
#define CHROMA_JSON_SCENE_H

#include <serialization/formats/Json.h>
#include <common/PrecompiledHeader.h>
#include <uid/UID.h>

class JSONScene : public JSON
{
public:

	void AddNewEntity(const UID& uid);
	void AddNewComponent(const UID& uid);



	JSONScene();
	~JSONScene() {};

private:
	rapidjson::Value m_RootObject{ rapidjson::kObjectType };
	rapidjson::Value m_LevelObject{ rapidjson::kObjectType };
	rapidjson::Value m_EntitiesObject{ rapidjson::kObjectType };
	rapidjson::Value m_ComponentsObject{ rapidjson::kObjectType };
};

#define CHROMA_ROOT "Root"
#define CHROMA_LEVEL "Level"
#define CHROMA_ENTITIES "Entities"
#define CHROMA_COMPONENTS "Components"

#endif