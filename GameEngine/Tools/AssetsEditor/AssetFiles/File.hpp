#pragma once

#include <string>
#include <filesystem>
#include <functional>

namespace AGE
{
	namespace AE
	{
		class Folder;

		typedef std::tr2::sys::path Path;

		class AssetFile : std::enable_shared_from_this<AssetFile>
		{
		public:
			AssetFile() = delete;
			AssetFile(const std::tr2::sys::path &path, Folder *parent = nullptr);

			template <typename T>
			void update(std::function<void(T*)> &function)
			{
				std::static_assert(std::is_convertible<T*, AssetFile*>::value, "Error : T should inherit from AssetFile.");
				function(static_cast<T*>(this));
			}

			virtual ~AssetFile();

			bool _active;
			bool _selected;
			struct tm _timeinfo;
			int _type;

		private:
			const std::tr2::sys::path _path;
			Folder *_folder;
		};
	}
}