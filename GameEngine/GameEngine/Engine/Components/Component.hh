#pragma once

#include <string>
#include <Entities/Entity.hh>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <regex>

namespace	Component
{
	struct Base
	{
		Base(std::size_t serId);
		virtual ~Base();
		virtual Base &operator=(const Base &other);
		void			setEntity(const Entity &entity);
		Entity		&getEntity();
		virtual void reset() = 0;
		std::size_t serializedID;

		virtual void unserialize(cereal::JSONInputArchive &ar) = 0;
		virtual void unserialize(cereal::BinaryInputArchive &ar) = 0;
		virtual void unserialize(cereal::XMLInputArchive &ar) = 0;
		virtual void unserialize(cereal::PortableBinaryInputArchive &ar) = 0;
		virtual void _serialize(cereal::JSONOutputArchive &ar) = 0;
		virtual void _serialize(cereal::BinaryOutputArchive &ar) = 0;
		virtual void _serialize(cereal::XMLOutputArchive &ar) = 0;
		virtual void _serialize(cereal::PortableBinaryOutputArchive &ar) = 0;

		template <typename Archive>
		void serializeBase(Archive &ar)
		{
			ar(cereal::make_nvp("Component_Type_ID", serializedID));
			_serialize(ar);
		}

	protected:
		Entity				_entity;
		static std::size_t uniqueId()
		{
			static std::size_t id = 0;
			return id++;
		}
	};

	template <class T>
	struct ComponentBase : public Base
	{
		ComponentBase()
			: Base(typeid(T).hash_code())
			, _name(typeid(T).name())
		{
		}

		virtual ~ComponentBase()
		{
		}

		static std::size_t getTypeId()
		{
			static std::size_t id = uniqueId();
			return id;
		}

		virtual void unserialize(cereal::JSONInputArchive &ar)
		{
			ar(*(dynamic_cast<T*>(this)));
		}

		virtual void unserialize(cereal::BinaryInputArchive &ar)
		{
			ar(*(dynamic_cast<T*>(this)));
		}

		virtual void unserialize(cereal::XMLInputArchive &ar)
		{
			ar(*(dynamic_cast<T*>(this)));
		}

		virtual void unserialize(cereal::PortableBinaryInputArchive &ar)
		{
			ar(*(dynamic_cast<T*>(this)));
		}

		virtual void _serialize(cereal::JSONOutputArchive &ar)
		{
			ar(cereal::make_nvp(_name, *dynamic_cast<T*>(this)));
		}

		virtual void _serialize(cereal::BinaryOutputArchive &ar)
		{
			ar(*dynamic_cast<T*>(this));
		}

		virtual void _serialize(cereal::XMLOutputArchive &ar)
		{
			ar(cereal::make_nvp(_name, *dynamic_cast<T*>(this)));
		}

		virtual void _serialize(cereal::PortableBinaryOutputArchive &ar)
		{
			ar(*dynamic_cast<T*>(this));
		}

	private:
		const std::string _name;
	};
}