#include "AssetsDatabase.hpp"
#include <Utils/FileSystem.hpp>

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
			if (!AGE::FileSystem::CreateFolder(rawFolder))
			{
				return false;
			}
			if (!AGE::FileSystem::CreateFolder(cookedFolder))
			{
				return false;
			}
			if (AGE::FileSystem::Exists(saveFile))
			{
				// load database
			}
			_saveFile = saveFile;
		}

		bool AssetsDatabase::init(const std::string &saveFile)
		{
			reset();
			if (AGE::FileSystem::Exists(saveFile))
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