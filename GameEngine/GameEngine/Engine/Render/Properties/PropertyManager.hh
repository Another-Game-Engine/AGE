#pragma once

#include <vector>
#include <memory>
#include <Render/Key.hh>

namespace AGE
{
	class Properties;

	class PropertyManager
	{
	public:
		PropertyManager();
		PropertyManager(PropertyManager &&);
		PropertyManager(PropertyManager const &) = delete;
		PropertyManager &operator=(PropertyManager const &) = delete;

	public:
		Key<Properties> add_properties(std::shared_ptr<Properties> const &);
		std::shared_ptr<Properties> get_properties(Key<Properties> const &);

	private:
		std::vector<std::shared_ptr<Properties>> _properties_array;
	};
}