#ifndef   __ASSETS_LOADER_HPP__
# define  __ASSETS_LOADER_HPP__

#include <Managers/AssetsManager.hpp>
#include <MediaFiles/AMediaFile.hpp>
#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/TextureFile.hpp>
#include <memory>

class AssetsLoader : public AssetsManager
{
public:
	AssetsLoader()
	{
		registerType<ObjFile>();
		registerType<MaterialFile>();
		registerType<TextureFile>();
	}

	virtual ~AssetsLoader()
	{}

	void loadFromList(const File &file)
	{
		assert(file.exists() == true && "Import file does not exist.");
		std::multimap<std::size_t, std::string> files;
		std::ifstream ifs(file.getFullName() , std::ios_base::binary);
		cereal::JSONInputArchive ar(ifs);
		ar(files);
		ifs.close();
		for (auto &e : files)
		{
			loadFromPath(File(e.second));
		}
	}

	void loadFromPath(const File &file)
	{
		assert(file.exists() == true && "File does not exist.");
		std::ifstream ifs(file.getFullName(), std::ios_base::binary);
		std::shared_ptr<AMediaFile> media = unserializeFromStream<cereal::PortableBinaryInputArchive>(ifs);
		assert(media.get() != nullptr && "Media fail to load.");
		_files.insert(std::make_pair(media->name, media));
	}

private:
	std::map<std::string, std::shared_ptr<AMediaFile>> _files;
};

#endif   //__ASSETS_LOADER_HPP__