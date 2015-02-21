#include <Render/Properties/Properties.hh>

namespace AGE
{

	Properties::Properties(std::vector<std::shared_ptr<IProperty>> const &properties) :
		_properties(properties)
	{
	}

	Properties::Properties(Properties &&other) :
		_properties(std::move(other._properties))
	{
	}

	Key<Property> Properties::add_property(std::shared_ptr<IProperty> const &prop)
	{
		_properties.emplace_back(prop);
		return (Key<Property>::createKey(int(_properties.size()) - 1));
	}

	void Properties::update_properties(std::shared_ptr<Program> const &p) const
	{
		for (auto property : _properties) {
			property->update(p);
		}
	}
}