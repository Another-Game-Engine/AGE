#include <Managers/AssetsManager.hpp>
#include <MediaFiles/AMediaFile.hpp>

	AssetsManager::AssetsManager()
	{
		AMediaFile::setManager(this);
	}

	AssetsManager::~AssetsManager()
	{}

	void AssetsManager::add(std::shared_ptr<AMediaFile> f)
	{
		if (_files.find(f->name) != std::end(_files))
			return;
		_files.insert(std::make_pair(f->name, f));
	}
