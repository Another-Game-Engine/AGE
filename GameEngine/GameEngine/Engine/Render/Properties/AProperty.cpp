#include <Render/Properties/AProperty.hh>

AProperty::AProperty(std::string &&name, std::vector<std::shared_ptr<Program>> const &programs) :
_name(std::move(name))
{
	for (auto &program : programs) {
		_resources.emplace_back(std::make_pair(program, nullptr/*program->get_resource<IProgramResources>(_name)*/));
	}
}

AProperty::AProperty(AProperty &&move) :
_name(std::move(move._name)),
_resources(std::move(_resources))
{

}

std::string const & AProperty::name() const
{
	return (_name);
}

IProgramResources * AProperty::get_resource(std::shared_ptr<Program> const &p)
{
	for (auto &resource : _resources) {
		if (*resource.first == *p) {
			return (resource.second.get());
		}
	}
	return (nullptr);
}
