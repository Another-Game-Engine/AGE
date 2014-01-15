#include <Managers/AssetsConvertorManager.hh>
#include <Managers/AConvertor.hh>
#include <MediaFiles/AMediaFile.hpp>
#include <Managers/ObjConvertor.hh>

#include <MediaFiles/AMediaFile.hpp>
#include <MediaFiles/ObjFile.hpp>

AssetsConvertorManager::AssetsConvertorManager()
{
	registerConvertor<ObjConvertor>();
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
	//std::ofstream test;
	//Archive::open(test, "test.serialization");
	//for (auto &e : _files)
	//{
	//	e.second->serialize(test);
	//}

	std::ifstream test;
	Archive::open(test, "test.serialization");
	AMediaFile t;
	for (auto &e : _files)
	{
		e.second->unserialize(test);
	}
	return true;
}