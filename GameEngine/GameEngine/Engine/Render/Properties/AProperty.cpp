#include <Render/Properties/AProperty.hh>
#include <Render/Program.hh>

namespace AGE
{
	AProperty::AProperty(std::string &&name) :
		_name(std::move(name))
#if AGE_DEBUG
		, _shaderVersion(0)
#endif
	{

	}

	AProperty::AProperty(AProperty &&move) :
		_name(std::move(move._name)),
		_registered_resources(std::move(move._registered_resources))
	{

	}

	std::string const & AProperty::name() const
	{
		return (_name);
	}

	std::shared_ptr<IProgramResources> AProperty::get_resource(std::shared_ptr<Program> const &program)
	{
#ifdef AGE_DEBUG
		auto version = program->getVersion();
		if (version != _shaderVersion)
		{
			_shaderVersion = version;
			_registered_resources.clear();
		}
#endif
		for (auto &resource : _registered_resources)
		{
			if (*program == *resource.first)
			{
				return resource.second;
			}
		}
		auto resource = program->get_resource_interface(_name);
		_registered_resources.emplace_back(std::make_pair(program, resource));
		return (resource);
	}

}