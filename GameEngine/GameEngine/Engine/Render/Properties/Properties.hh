#pragma once

#include <vector>
#include <memory>
#include <Render/Properties/IProperty.hh>
#include <Render/Key.hh>

namespace AGE
{

	class Properties
	{
	public:
		Properties(std::vector<std::shared_ptr<IProperty>> const &properties);
		Properties(Properties &&other);
		Properties(Properties const &) = default;
		Properties &operator=(Properties const &) = default;
		~Properties() = default;

	public:
		Key<Property> add_property(std::shared_ptr<IProperty> const &prop);
	
	public:
		template <typename type_t> std::shared_ptr<type_t> const &get_property(Key<Property> const &key);

	private:
		std::vector<std::shared_ptr<IProperty>> _properties;
	};

	template <typename type_t>
	std::shared_ptr<type_t> const &Properties::get_property(Key<Property> const &key)
	{
		return (std::static_pointer_cast<type_t>(_properties[key.getId()]));
	}

}