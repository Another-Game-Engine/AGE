#include <Render/Properties/PropertyManager.hh>
#include <Render/Properties/Properties.hh>

namespace AGE
{
	PropertyManager::PropertyManager()
	{

	}

	PropertyManager::PropertyManager(PropertyManager &&other) :
		_properties_array(std::move(other._properties_array))
	{

	}

	Key<Properties> PropertyManager::add_properties(std::shared_ptr<Properties> const &properties)
	{
		return (Key<Properties>::createKey(_properties_array.alloc(properties)));
	}

	void PropertyManager::remove_properties(Key<Properties> const &toRm)
	{
		_properties_array.dealloc(toRm.getId());
	}

	std::shared_ptr<Properties> PropertyManager::get_properties(Key<Properties> const &key) const
	{
		return (_properties_array.get(key.getId()));
	}
}