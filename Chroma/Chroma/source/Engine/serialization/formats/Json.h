#ifndef _CHROMA_JSON_H_
#define _CHROMA_JSON_H_

//common
#include <common/PrecompiledHeader.h>

//rapidjson
#include <rapidJSON/document.h>
#include <rapidJSON/writer.h>
#include <rapidJSON/filereadstream.h>
#include <rapidJSON/filewritestream.h>
#include <rapidJSON/stringbuffer.h>

//chroma
#include <common/CoreCommon.h>


class JSON
{
public:
	JSON(const JSON&) {};
	JSON& operator=(const JSON&) {};
	JSON( const char* sourcePath, bool isFile=true);
	~JSON() {};

	bool HasKey(const char* keyString);

	rapidjson::Value& GetValue(const char* accessorString) {
		return m_Document[accessorString];
	}

	rapidjson::Document& GetDocument() { return m_Document; }

	static void Write(JSON& sourceJson, const char* destinationPath);
	void Write(const char* destinationPath);

	friend std::ostream& operator<<(std::ostream& out, JSON& refJson);

private:
	rapidjson::Document m_Document;
	const char* m_SourcePath;
	void Load(const char* sourcePath);
	bool validSourceFile(const char* sourcePath);
};


inline std::ostream& operator<<(std::ostream& out, JSON& refJson)
{
	rapidjson::StringBuffer stringBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> stringWriter(stringBuffer);
	refJson.GetDocument().Accept(stringWriter);

	out << stringBuffer.GetString();
	return out;
}

#endif