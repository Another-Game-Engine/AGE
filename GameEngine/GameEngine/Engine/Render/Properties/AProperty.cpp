#include <Render/Properties/AProperty.hh>
#include <Render/Program.hh>
#include <Utils/Profiler.hpp>

namespace AGE
{
	AProperty::AProperty(std::string &&name) :
		IProperty(std::move(name))
	{
#ifdef AGE_DEBUG
		_shaderVersion = 0;
#endif
	}

	AProperty::AProperty(AProperty &&move) :
		IProperty(std::move(std::string(move._name))),
		_registered_resources(std::move(move._registered_resources))
	{

	}

	std::shared_ptr<IProgramResources> AProperty::get_resource(std::shared_ptr<Program> const &program)
	{
		SCOPE_profile_cpu_function("RenderTimer");
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