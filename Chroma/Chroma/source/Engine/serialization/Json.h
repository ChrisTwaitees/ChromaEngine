#ifndef _CHROMA_JSON_H_
#define _CHROMA_JSON_H_

//stl
#include <string>

#include <fstream>

//rapidjson

#include <rapidJSON/document.h>
#include <rapidJSON/writer.h>
#include <rapidJSON/filereadstream.h>
#include <rapidJSON/filewritestream.h>

//chroma
#include <common/CoreCommon.h>


class Json
{
public:
	Json(const Json&) {};
	Json& operator=(const Json&) {};
	Json( const char* sourcePath);
	~Json() {};

	template<typename T>
	T operator[](const char* accessorString) {
		rapidjson::Value& value = m_Document[accessorString];
		return value.Get();
	}

	template<typename T>
	T Get(const char* accessorString) {
		rapidjson::Value& value = m_Document[accessorString];
		return value.Get();
	}

	void Load(const char* sourcePath);
	void Write(const char* destinationPath);

private:
	rapidjson::Document m_Document;
	bool validSourceFile(const char* sourcePath);
};

namespace Chroma
{
	class JsonLoader
	{
		static Json LoadJson(const char* sourcePath);
	};

}

#endif