#include <Managers/AssetsConvertorManager.hh>
#include <Managers/AConvertor.hh>
#include <Managers/ObjConvertor.hh>
#include <Managers/MaterialConvertor.hpp>
#include <Managers/FileTypeRegister.hpp>

#include <MediaFiles/MaterialFile.hpp>

AssetsConvertorManager::AssetsConvertorManager()
{
        registerConvertor<ObjConvertor>();
		registerConvertor<MaterialConvertor>();
        FileTypeRegister::getInstance()->registerType<ObjFile>();
        FileTypeRegister::getInstance()->registerType<MaterialFile>();
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
        std::ofstream ofs("test.serialization", std::ios_base::binary);

        for (auto &e : _files)
        {
                e.second->serialize<cereal::PortableBinaryOutputArchive>(ofs);
        }
        ofs.close();
        std::ifstream ifs("test.serialization", std::ios_base::binary);

        for (auto &e : _files)
        {
                AMediaFile *test = FileTypeRegister::getInstance()->unserializeFromStream<cereal::PortableBinaryInputArchive>(ifs);
                std::cout << "lol" << std::endl;
        }
        return true;
}