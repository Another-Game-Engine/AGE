#include <vector>
#include <string>
#include <iostream>
#include <windows.h>

#include "AConfigurationFile.hh"

namespace AGE
{
	AConfigurationFile::AConfigurationFile(const std::string &fileName)
	{

	}

	AConfigurationFile::~AConfigurationFile()
	{
	}

	bool AConfigurationFile::readFile()
	{

		return true;
	}

	bool AConfigurationFile::listDir(const std::string &path, std::vector<std::string> &files)
	{
		WIN32_FIND_DATAA data;
		HANDLE hFile = FindFirstFileA(path.c_str(), &data);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			std::cerr << "error" << std::endl;
			return false;
		}
		do 
		{
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				files.push_back(data.cFileName);
		} while (FindNextFileA(hFile, &data) != 0 || GetLastError() != ERROR_NO_MORE_FILES);
		FindClose(hFile);
		return true;
	}
} // namespace AGE