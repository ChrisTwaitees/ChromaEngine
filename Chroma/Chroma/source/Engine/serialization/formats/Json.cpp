#include "Json.h"

JSON::JSON(const char* source, bool isFile)
{
	if(isFile)
		Load(source);
	else
	{
		m_Document.Parse(source);
	}

}

bool JSON::HasKey(const char* keyString)
{
	return m_Document.HasMember(keyString);
}


void JSON::Load(const char* sourcePath)
{
	// Parse a JSON string into DOM.
	// Open JSON from sourcePath
	if (validSourceFile(sourcePath))
	{
		m_SourcePath = sourcePath;
		FILE* jsonRawFilePtr = fopen(sourcePath, "rb"); // non-Windows use "w"

		char readBuffer[65536];
		rapidjson::FileReadStream jsonFileStream(jsonRawFilePtr, readBuffer, sizeof(readBuffer));

		// Parse to DOM
		const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
		m_Document.ParseStream(jsonFileStream);

		// close the file
		fclose(jsonRawFilePtr);

		// debug
		CHROMA_INFO_UNDERLINE;
		CHROMA_INFO("JSON LOADER :: Successfully loaded JSON from {0}", sourcePath);
		CHROMA_INFO_UNDERLINE;
	}
	else
	{
		CHROMA_ERROR("JSON LOADER :: Source file : {0} is not a valid path.", sourcePath);
	}
}

void JSON::Write(JSON& sourceJson, const char* destinationPath)
{
	FILE* fp = fopen(destinationPath, "wb"); // non-Windows use "w"

	char writeBuffer[65536];
	rapidjson::FileWriteStream jsonFileOutStream(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Writer<rapidjson::FileWriteStream> writer(jsonFileOutStream);
	sourceJson.m_Document.Accept(writer);

	fclose(fp);

	CHROMA_INFO_UNDERLINE;
	CHROMA_INFO("JSON LOADER :: Successfully wrote JSON to {0}", destinationPath);
	CHROMA_INFO_UNDERLINE;
}

void JSON::Write(const char* destinationPath)
{
	FILE* fp = fopen(destinationPath, "wb"); // non-Windows use "w"

	char writeBuffer[16384];
	rapidjson::FileWriteStream jsonFileOutStream(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Writer<rapidjson::FileWriteStream> writer(jsonFileOutStream);
	m_Document.Accept(writer);

	fclose(fp);

	CHROMA_INFO_UNDERLINE;
	CHROMA_INFO("JSON LOADER :: Successfully wrote JSON to {0}", destinationPath);
	CHROMA_INFO_UNDERLINE;
}

bool JSON::validSourceFile(const char* sourcePath)
{
	std::ifstream f(sourcePath);
	return f.good();
}


