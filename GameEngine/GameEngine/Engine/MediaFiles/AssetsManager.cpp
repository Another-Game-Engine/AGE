#include <MediaFiles/AssetsManager.hpp>
#include <Core/Engine.hh>

AssetsManager::AssetsManager()
{
}

AssetsManager::~AssetsManager()
{}

void AssetsManager::init()
{
	// TODO DELETE THIS LINE AND AMEDIAFILE MANAGER PTR
	//AMediaFile::setManager(shared_from_this());
}

void AssetsManager::add(std::shared_ptr<AMediaFile> f)
{
	if (_files.find(f->name) != std::end(_files))
		return;
	_files.insert(std::make_pair(f->name, f));
}