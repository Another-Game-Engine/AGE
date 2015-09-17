#pragma once

#include <string>
#include <filesystem>
#include <functional>

namespace AGE
{
	namespace FileUtils
	{
		class Folder;

		typedef std::tr2::sys::path Path;

		class AssetFile : std::enable_shared_from_this < AssetFile >
		{
		public:
			AssetFile() = delete;
			AssetFile(const std::tr2::sys::path &path, Folder *parent = nullptr);

			template <typename T>
			void update(std::function<void(T*)> &function)
			{

				static_assert(std::is_convertible<T*, AssetFile*>::value, "T have to inherit from AssetFile.");
				function(static_cast<T*>(this));
			}

			virtual ~AssetFile();

			bool _active;
			bool _selected;
			struct tm _lastWriteTime;
			std::string _lastWriteTimeStr;
			int _type;
			inline const std::string &getPath() const { return _pathStr; }
			inline const std::string &getFileName() const { return _fileName; }
			inline std::tr2::sys::path getFsPath() const { return _path; }
			inline std::shared_ptr<AssetFile> getSharedPtrOnThis() { return shared_from_this(); }

		private:
			const std::tr2::sys::path _path;
			std::string _pathStr;
			std::string _fileName;
			Folder *_folder;
		};
	}
}