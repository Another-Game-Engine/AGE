#include "AMediaFile.hpp"

void FileTypeRegister::registerType(AMediaFile *t)
{
	std::size_t key = t->type;
	if (refs.find(key) != std::end(refs))
		return;
	refs.insert(std::make_pair(key, t));
}

AMediaFile *FileTypeRegister::getFromType(std::size_t key, std::ifstream &s)
{
	auto &it = refs.find(key);
	if (it == std::end(refs))
		return nullptr;
	return it->second->unserialize(s);
}

AMediaFile *FileTypeRegister::unserializeFromStream(std::ifstream &s)
{
	cereal::JSONInputArchive ar(s);
	std::size_t key = 0;
	ar(key);
	return getFromType(key, s);
}