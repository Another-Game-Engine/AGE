#include <Managers/AssetsConvertorManager.hh>
#include <Managers/AConvertor.hh>

AssetsConvertorManager::AssetsConvertorManager()
{}

AssetsConvertorManager::~AssetsConvertorManager()
{}

void AssetsConvertorManager::setOutputDirectory(const std::string directory)
{
	_outputDirectory = File(directory);
}
