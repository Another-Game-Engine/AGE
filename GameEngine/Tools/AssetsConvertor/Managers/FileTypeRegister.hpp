#ifndef  __FILE_TYPE_REGISTER_HPP__
# define __FILE_TYPE_REGISTER_HPP__

#include <map>
#include <MediaFiles/MediaFile.hpp>

struct FileTypeRegister
{
	static FileTypeRegister *getInstance()
	{
		static FileTypeRegister *t = new FileTypeRegister();
		return t;
	}
	std::map<std::size_t, AMediaFile*> refs;


	template <typename Archive>
	AMediaFile *getFromType(std::size_t key, Archive &s)
	{
		auto &it = refs.find(key);
		if (it == std::end(refs))
			return nullptr;
		return it->second->unserialize(s);
	}

	template <typename Archive>
	AMediaFile *unserializeFromStream(std::ifstream &s)
	{
		std::size_t key = 0;
		Archive ar(s);
		ar(key);
		return getFromType(key, ar);
	}

	template <typename T>
	void registerType()
	{
		auto instance = new T();
		std::size_t key = instance->type;
		if (refs.find(key) != std::end(refs))
			return;
		refs.insert(std::make_pair(key, instance));
	}
};

#endif  //__FILE_TYPE_REGISTER_HPP__