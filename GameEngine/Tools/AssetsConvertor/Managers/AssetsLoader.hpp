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
		AMediaFile::setManager(this);
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
			AMediaFile::loadFromFile<cereal::PortableBinaryInputArchive>(File(e.second));
		}
	}

private:
	std::map<std::string, std::shared_ptr<AMediaFile>> _files;
};

#endif   //__ASSETS_LOADER_HPP__