#include <Render/Properties/AProperty.hh>
#include <Render/Program.hh>

AProperty::AProperty(std::string &&name) :
_name(std::move(name))
{

}

AProperty::AProperty(std::string &&name, std::vector<std::shared_ptr<Program>> const &programs) :
_name(std::move(name))
{
	for (auto &program : programs) {
		_resources.emplace_back(std::make_pair(program, program->get_resource_interface(_name)));
	}
}

AProperty::AProperty(AProperty &&move) :
_name(std::move(move._name)),
_resources(std::move(move._resources))
{

}

std::string const & AProperty::name() const
{
	return (_name);
}

std::shared_ptr<IProgramResources> const & AProperty::get_resource(std::shared_ptr<Program> const &p)
{
	for (auto &resource : _resources) {
		if (*resource.first == *p) {
			return (resource.second);
		}
	}
	return (nullptr);
}
