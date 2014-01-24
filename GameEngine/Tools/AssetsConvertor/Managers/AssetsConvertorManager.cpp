#include <Managers/AssetsConvertorManager.hh>
#include <Managers/AConvertor.hh>
#include <Managers/ObjConvertor.hh>
#include <Managers/MaterialConvertor.hpp>
#include <Managers/TgaConvertor.hpp>

#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/TextureFile.hpp>

AssetsConvertorManager::AssetsConvertorManager()
{
	registerConvertor<ObjConvertor>();
	registerConvertor<MaterialConvertor>();
	registerConvertor<TgaConvertor>();
	registerType<ObjFile>();
	registerType<MaterialFile>();
	registerType<TextureFile>();
}

AssetsConvertorManager::~AssetsConvertorManager()
{}

void AssetsConvertorManager::setOutputDirectory(const std::string directory)
{
	_outputDirectory = File(directory);
}

std::shared_ptr<AMediaFile> AssetsConvertorManager::load(const std::string filename, const std::string name)
{
	File file(filename);

	for (auto &e : _convertors)
	{
		if (e.second->supportFile(file))
		{
			if (_files.find(name) != std::end(_files))
				return _files[name];
			auto res = e.second->convert(file);
			if (res.get() == nullptr)
				return std::shared_ptr<AMediaFile>{nullptr};
			res->name = name;
			res->path = file.getFullName();
			_files.insert(std::make_pair(name, res));
			return res;
		}
	}
	return std::shared_ptr<AMediaFile>{nullptr};
}

bool AssetsConvertorManager::serializeData()
{
	for (auto &e : _files)
	{
		std::ofstream ofs(e.second->name + ".cpd", std::ios_base::binary);
		e.second->serialize<cereal::BinaryOutputArchive>(ofs);
		ofs.close();
	}

	for (auto &e : _files)
	{
		std::ifstream ifs(e.second->name + ".cpd", std::ios_base::binary);
		AMediaFile *test = unserializeFromStream<cereal::BinaryInputArchive>(ifs);
		ifs.close();
	}
	return true;
}