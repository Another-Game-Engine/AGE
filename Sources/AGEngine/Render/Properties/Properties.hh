#pragma once

#include <vector>
#include <memory>
#include <Render/Properties/IProperty.hh>
#include <Utils/Key.hh>
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
		virtual ~Properties();

	public:
		Key<Property> add_property(std::shared_ptr<IProperty> const &prop);
		Key<Property> add_property(std::shared_ptr<IProperty> &prop);
		void remove_property(Key<Property> const &prop);
		void merge_properties(const Properties &other);
		std::size_t getProgramId(std::size_t programId);
		inline std::size_t getHash() { if (_hashToRefresh){ _computeHash(); }; return _shaderHash; }
		void setProgramId(std::size_t programUniqueId, std::size_t givenId);
		bool empty() const { return _properties.empty(); }
	public:
		template <typename type_t> std::shared_ptr<type_t> get_property(Key<Property> const &key) const;

		std::size_t getPropertyIndex(const std::string &name) const
		{
			std::size_t res = 0;
			for (auto &e : _properties)
			{
				if (e->name() == name)
				{
					return res;
				}
				++res;
			}
			return -1;
		}

		// TODO remove
		std::shared_ptr<IProperty> &getProperty(const std::string &name)
		{
			int res = 0;
			for (auto &e : _properties)
			{
				if (e->name() == name)
				{
					return _properties[res];
				}
				++res;
			}
			static std::shared_ptr<IProperty> nullValue(nullptr);
			return nullValue;
		}

		inline std::shared_ptr<IProperty> &getProperty(const std::size_t &index) 
		{
			return _properties[index];
		}

	private:
		void _computeHash();

		std::vector<std::shared_ptr<IProperty>> _properties;
		std::vector<std::size_t> _shadersGivenId;
		std::size_t _shaderHash;
		bool        _hashToRefresh;
	};

	template <typename type_t>
	std::shared_ptr<type_t> Properties::get_property(Key<Property> const &key) const
	{
		return (std::static_pointer_cast<type_t>(_properties[key.getId()]));
	}
}