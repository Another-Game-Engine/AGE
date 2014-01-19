#include <Managers/AssetsConvertorManager.hh>
#include <Managers/AConvertor.hh>
#include <MediaFiles/AMediaFile.hpp>
#include <Managers/ObjConvertor.hh>

AssetsConvertorManager::AssetsConvertorManager()
{
	registerConvertor<ObjConvertor>();
	FileTypeRegister::getInstance()->registerType<ObjFile>();
}

AssetsConvertorManager::~AssetsConvertorManager()
{}

void AssetsConvertorManager::setOutputDirectory(const std::string directory)
{
	_outputDirectory = File(directory);
}

bool AssetsConvertorManager::load(const std::string filename, const std::string name)
{
	File file(filename);

	for (auto &e : _convertors)
	{
		if (e.second->supportFile(file))
		{
			auto res = e.second->convert(file);
			if (res.get() == nullptr)
				return false;
			res->name = name;
			res->path = file.getFullName();
			_files.insert(std::make_pair(name, res));
			return true;
		}
	}
	return false;
}

bool AssetsConvertorManager::serializeData()
{
	std::ofstream ofs("test.serialization");

	for (auto &e : _files)
	{
		e.second->serialize(ofs);
//		cereal::JSONOutputArchive oa(ofs);



		//e.second->serialize(oa);
		//oa(e.second);
		//test1->serialize(oa);
		//test2->serialize(oa);
		//test3->serialize(oa);
		//oa(*test3.get());
	}
	ofs.close();
	std::ifstream ifs("test.serialization");

	for (auto &e : _files)
	{
//		e.second->unserialize(ifs);
	}
	return true;
}