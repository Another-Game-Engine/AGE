#ifndef   __ASSETS_LOADER_HPP__
# define  __ASSETS_LOADER_HPP__

#include <Managers/AssetsManager.hpp>
#include <MediaFiles/AMediaFile.hpp>
#include <memory>

class AssetsLoader : public AssetsManager
{
public:
	AssetsLoader()
	{}

	virtual ~AssetsLoader()
	{}

	void loadFromList(const File &file)
	{
		assert(file.exists() == true && "Import file do not exists.");
	}

private:
	std::map<std::string, std::shared_ptr<AMediaFile>> _files;
};

#endif   //__ASSETS_LOADER_HPP__