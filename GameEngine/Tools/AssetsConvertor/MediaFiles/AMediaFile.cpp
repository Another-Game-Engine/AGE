#include "AMediaFile.hpp"

void FileTypeRegister::registerType(AMediaFile *t)
{
	std::size_t key = t->type;
	if (refs.find(key) != std::end(refs))
		return;
	refs.insert(std::make_pair(key, t));
}

AMediaFile *FileTypeRegister::getFromType(std::size_t key, cereal::JSONInputArchive &s)
{
	auto &it = refs.find(key);
	if (it == std::end(refs))
		return nullptr;
	return it->second->unserialize(s);
}

AMediaFile *FileTypeRegister::unserializeFromStream(std::ifstream &s)
{
	std::size_t key = 0;
	cereal::JSONInputArchive ar(s);
	ar(key);
	return getFromType(key, ar);
}