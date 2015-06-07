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
	class AssetsManager;

	struct MaterialInstance
	{
		std::vector<std::shared_ptr<Property>> _properties;
	};

	struct MaterialSetInstance
	{
	public:
		MaterialSetInstance()
		{
			_valid = false;
		}
		std::string name;
		std::string path;
		std::vector<MaterialInstance> datas;

		bool isValid()
		{
			return _valid;
		}
	private:
		std::atomic<bool> _valid;
		friend class AssetsManager;
	};

}

