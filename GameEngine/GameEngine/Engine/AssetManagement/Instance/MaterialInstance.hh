#pragma once

#include <string>
#include <vector>
#include <memory>
//#include <map>
#include <Render/Key.hh>
#include <Threads/RenderThread.hpp>
#include <Render/Properties/Properties.hh>

namespace AGE
{
	struct MaterialInstance
	{
		//Key<Material> _material_key;
		//std::map<std::string, Key<Property>> _properties;
		std::vector<std::shared_ptr<Property>> _properties;
	};

	struct MaterialSetInstance
	{
	public:
		std::string name;
		std::string path;
		std::vector<MaterialInstance> datas;
	};

}

