#include "FileHelper.h"

namespace Chroma
{
	std::vector<std::string> FileHelper::GetFilesFromDirectory(std::string const& directoryPath)
	{
		std::vector<std::string> fileNames;
		std::string pattern(directoryPath);
		pattern.append("\\*");
		WIN32_FIND_DATA data;
		HANDLE hFind;
		if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
			do {
				if (FileExists(directoryPath + "/" + data.cFileName))
					fileNames.push_back(data.cFileName);
			} while (FindNextFile(hFind, &data) != 0);
			FindClose(hFind);
		}

		return fileNames;
	}

	bool FileHelper::FileExists(std::string const& filePath)
	{
		std::ifstream f(filePath.c_str());
		return f.good();
	}

}