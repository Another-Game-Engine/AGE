#include "AssetsDatabase.hpp"
#include <FileUtils/FileUtils/FileSystemHelpers.hpp>

namespace AGE
{
	namespace AE
	{
		AssetsDatabase::AssetsDatabase()
		{
			reset();
		}

		AssetsDatabase::~AssetsDatabase()
		{
		}

		bool AssetsDatabase::init(const std::string &rawFolder, const std::string &cookedFolder, const std::string &saveFile)
		{
			reset();
			if (!FileUtils::AgeCreateFolder(rawFolder))
			{
				return false;
			}
			if (!FileUtils::AgeCreateFolder(cookedFolder))
			{
				return false;
			}
			if (FileUtils::AgeExists(saveFile))
			{
				// load database
			}
			_saveFile = saveFile;
			return true;
		}

		bool AssetsDatabase::init(const std::string &saveFile)
		{
			reset();
			if (FileUtils::AgeExists(saveFile))
			{
				// load database
			}
			else
				return false;
			_saveFile = saveFile;
			return true;
		}

		void AssetsDatabase::reset()
		{
			_raws = nullptr;
			_cooked = nullptr;
			_saveFile = "";
		}
	}
}