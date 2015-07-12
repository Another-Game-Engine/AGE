#pragma once

#include <vector>
#include <memory>
#include <Render/Properties/IProperty.hh>
#include <Render/Key.hh>
#include <Utils/RWLock.hpp>

namespace AGE
{

	class Properties
	{
	public:
		Properties() = default;
		Properties(std::vector<std::shared_ptr<IProperty>> const &properties);
		Properties(Properties &&other);
		Properties(Properties const &);
		Properties &operator=(Properties const &) = delete;
		~Properties() = default;

	public:
		Key<Property> add_property(std::shared_ptr<IProperty> const &prop);
		void remove_property(Key<Property> const &prop);
		void update_properties(std::shared_ptr<Program> const &p) const;
		void merge_properties(const Properties &other);
	public:
		template <typename type_t> std::shared_ptr<type_t> get_property(Key<Property> const &key) const;

		std::shared_ptr<IProperty> searchForProperty(const std::string &name) const
		{
			RWLockGuard lock(_lock, false);
			for (auto &e : _properties)
			{
				if (e->name() == name)
				{
					return e;
				}
			}
			return nullptr;
		}

	private:
		std::vector<std::shared_ptr<IProperty>> _properties;
		mutable RWLock _lock;
	};

	template <typename type_t>
	std::shared_ptr<type_t> Properties::get_property(Key<Property> const &key) const
	{
		RWLockGuard lock(_lock, false);
		return (std::static_pointer_cast<type_t>(_properties[key.getId()]));
	}
}