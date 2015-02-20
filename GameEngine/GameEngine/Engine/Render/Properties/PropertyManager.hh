#pragma once

#include <vector>
#include <memory>
#include <Render/Key.hh>
#include <Utils/MemoryPool.hpp>

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
		void remove_properties(Key<Properties> const &toRm);
		std::shared_ptr<Properties> get_properties(Key<Properties> const &) const;

	private:
		MemoryPool<std::shared_ptr<Properties>> _properties_array;
	};
}