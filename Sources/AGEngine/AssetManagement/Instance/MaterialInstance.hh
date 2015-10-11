#pragma once

#include <string>
#include <vector>
#include <memory>
#include <Threads/RenderThread.hpp>
#include <Render/Properties/Properties.hh>

namespace AGE
{
	class AssetsManager;

	struct MaterialInstance
	{
		std::vector<std::shared_ptr<Property>> _properties;

		// unique id (generated with a simple rand() for now, that's temporary)
		std::size_t _id;
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

