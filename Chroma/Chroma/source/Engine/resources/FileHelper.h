#ifndef _CHROMA_FILEHELPER_H_
#define _CHROMA_FILEHELPER_H_

#include <common/PrecompiledHeader.h>
#include <fstream>


namespace Chroma
{
	class FileHelper
	{
	public:
		static std::vector<std::string> GetFilesFromDirectory(std::string const& directoryPath);

		static bool FileExists(std::string const& filePath);
	};
}


#endif