#include <Managers/AConvertor.hh>
#include <Managers/AssetsConvertorManager.hh>

AConvertor::AConvertor(AssetsConvertorManager *manager, std::set<std::string> extensions)
: _manager(manager)
, _extensions(extensions)
{}

AConvertor::~AConvertor()
{}

bool AConvertor::supportFile(const File file)
{
	if (_extensions.find(file.getExtension()) != std::end(_extensions))
		return true;
	return false;
}