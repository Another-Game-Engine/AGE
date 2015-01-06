#include <Render/Properties/AProperty.hh>

AProperty::AProperty(std::string &&name, std::vector<std::shared_ptr<Program>> const &programs) :
_name(std::move(name))
{

}

AProperty::AProperty(AProperty &&move) :
_name(std::move(move._name)),
_resources(std::move(_resources))
{

}

std::string const & AProperty::name()
{
	return (_name);
}
