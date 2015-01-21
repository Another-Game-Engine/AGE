#pragma once

#include <Utils/Dependency.hpp>
#include <AssetFiles/Folder.hpp>
#include <AssetFiles/File.hpp>

namespace AGE
{
	namespace AE
	{
		class AssetsDatabase : public Dependency<AssetsDatabase>
		{
		public:
			AssetsDatabase();
			virtual ~AssetsDatabase();
			bool init(const std::string &rawFolder, const std::string &cookedFolder, const std::string &saveFile);
			bool init(const std::string &saveFile);
		private:
			void reset();
		private:
			std::unique_ptr<Folder> _raws;
			std::unique_ptr<Folder> _cooked;
			std::string _saveFile;
		};
	}
}