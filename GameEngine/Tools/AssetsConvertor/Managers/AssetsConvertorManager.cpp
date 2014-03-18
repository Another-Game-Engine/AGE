#include <Managers/AssetsConvertorManager.hh>
#include <Managers/AConvertor.hh>
#include <Managers/ObjConvertor.hh>
#include <Managers/MaterialConvertor.hpp>
#include <Managers/TgaConvertor.hpp>
#include <Managers/CubeMapConvertor.hpp>

#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/TextureFile.hpp>
#include <MediaFiles/CubeMapFile.hpp>

AssetsConvertorManager::AssetsConvertorManager()
{
	registerConvertor<ObjConvertor>();
	registerConvertor<MaterialConvertor>();
	registerConvertor<TgaConvertor>();
	registerConvertor<CubeMapConvertor>();
}

AssetsConvertorManager::~AssetsConvertorManager()
{}

void AssetsConvertorManager::setOutputDirectory(const std::string &directory)
{
	_outputDirectory = File(directory);
}

std::shared_ptr<AMediaFile> AssetsConvertorManager::load(const std::string &filename)
{
	const File file(filename);

	for (auto &e : _convertors)
	{
		if (e.second->supportFile(file))
		{
			std::string name = e.second->setName(file);
			if (_files.find(name) != std::end(_files))
				return _files[name];
			auto res = e.second->convert(file);
			if (res.get() == nullptr)
				return std::shared_ptr<AMediaFile>{nullptr};
			res->name = name;
			res->path = File(_outputDirectory.getFolder() + name + ".cpd");
			_files.insert(std::make_pair(name, res));
			return res;
		}
	}
	return std::shared_ptr<AMediaFile>{nullptr};
}

bool AssetsConvertorManager::serializeData(const std::string &exportName)
{
	std::multimap<std::size_t, std::string> files;
	for (auto &e : _files)
	{
		std::ofstream ofs(e.second->path.getFullName(), std::ios::trunc | std::ios::binary);
		e.second->serialize(ofs);
		ofs.close();
		files.insert(std::make_pair(e.second->getChilds(), e.second->path.getFullName()));
	}

	std::ofstream ofs(_outputDirectory.getFolder() + "export__" + exportName + ".cpd", std::ios::trunc);
	cereal::JSONOutputArchive ar(ofs);
	ar(files);

	//for (auto &e : _files)
	//{
	//	std::ifstream ifs(e.second->name + ".cpd", std::ios_base::binary);
	//	AMediaFile *test = unserializeFromStream<cereal::BinaryInputArchive>(ifs);
	//	ifs.close();
	//}
	return true;
}

void AssetsConvertorManager::clear()
{
	_files.clear();
}