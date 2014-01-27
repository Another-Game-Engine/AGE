#include <MediaFiles/AMediaFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/TextureFile.hpp>

AssetsManager *AMediaFile::_manager = nullptr;

void AMediaFile::loadFromList(const File &file)
{
	assert(file.exists() == true && "File does not exist.");

	std::ifstream ifs(file.getFullName(), std::ios::binary);
	cereal::JSONInputArchive ar(ifs);
	std::multimap<std::size_t, std::string> list;
	ar(list);
	for (auto &e : list)
	{
		AMediaFile::loadFromFile<cereal::BinaryInputArchive>(File(e.second));
	}
}
std::shared_ptr<AMediaFile> AMediaFile::get(const std::string &name)
{
	return _manager->get(name);
}
