#pragma once

#include <string>
#include <filesystem>
#include <array>
#include <set>

namespace AGE
{
	namespace AE
	{
		class Folder;

		typedef std::tr2::sys::path Path;

		class AssetFile : std::enable_shared_from_this<AssetFile>
		{
		public:
			enum PrintInfosTypes
			{
				Name = 1 << 0
				, Path = 1 << 1
				, Date = 1 << 2
				, Type = 1 << 3
				, Status = 1 << 4
				, END = 1 << 4
			};
			typedef int PrintInfos;

			AssetFile() = delete;
			AssetFile(const std::tr2::sys::path &path, const std::string &type, Folder *parent = nullptr);
			void printImgUi(AssetFile::PrintInfos infos = AssetFile::Name | AssetFile::Type, std::set<std::shared_ptr<AssetFile>> *list = nullptr);
			virtual ~AssetFile();
			std::string getDate();
			static std::string getExtension(const std::string &path);
			static std::string getExtension(const AE::Path &path);
		protected:
			std::array<float, 3> _color;
		private:
			bool _active;
			bool _selected;
			const std::tr2::sys::path _path;
			Folder *_folder;
			const std::string _type;
		};
	}
}