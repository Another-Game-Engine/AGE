#include <MediaFiles/AssetsManager.hpp>
#include <MediaFiles/AMediaFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/TextureFile.hpp>

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

std::shared_ptr<AMediaFile> AssetsManager::get(const File &file)
{
	std::string name = file.getShortFileName();
	auto it = _files.find(name);
	if (it == std::end(_files))
		return AMediaFile::loadFromFile<cereal::BinaryInputArchive>(file);
	return it->second;
}

std::shared_ptr<AMediaFile> AssetsManager::get(const std::string &name)
{
	auto it = _files.find(name);
	if (it == std::end(_files))
		return std::shared_ptr<AMediaFile>{nullptr};
	return it->second;
}