#include "Json.h"

Json::Json(const char* sourcePath)
{
	Load(sourcePath);
}


void Json::Load(const char* sourcePath)
{
	// Parse a JSON string into DOM.
	// Open JSON from sourcePath
	if (validSourceFile(sourcePath))
	{
		FILE* jsonRawFilePtr = fopen(sourcePath, "rb"); // non-Windows use "w"

		char readBuffer[16384];
		rapidjson::FileReadStream jsonFileStream(jsonRawFilePtr, readBuffer, sizeof(readBuffer));

		// Parse to DOM
		m_Document.ParseStream(jsonFileStream);

		// close the file
		fclose(jsonRawFilePtr);
	}
	else
	{
		CHROMA_ERROR("JSON::LOADER : Source file : {0} is not a valid path.", sourcePath);
	}
}

void Json::Write(const char* destinationPath)
{
	FILE* fp = fopen(destinationPath, "wb"); // non-Windows use "w"

	char writeBuffer[16384];
	rapidjson::FileWriteStream jsonFileOutStream(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Writer<rapidjson::FileWriteStream> writer(jsonFileOutStream);
	m_Document.Accept(writer);

	fclose(fp);
}

bool Json::validSourceFile(const char* sourcePath)
{
	std::ifstream f(sourcePath);
	return f.good();
}


namespace Chroma
{
	Json JsonLoader::LoadJson(const char* sourcePath)
	{
		return Json(sourcePath);
	}
}