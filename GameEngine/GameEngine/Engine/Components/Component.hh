#ifndef		__COMPONENT_HPP__
#define		__COMPONENT_HPP__

#include <string>
#include <Entities/Entity.hh>
#include <Core/AScene.hh>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <regex>

namespace	Component
{

	static int uniqueId()
	{
		static int id = 0;
		return id++;
	}

	struct Base
	{
		Base(std::size_t serId);
		virtual ~Base();
		virtual Base &operator=(const Base &other);
		void			setEntity(Entity entity);
		Entity		&getEntity();
		virtual void reset() = 0;
		std::size_t serializedID;

		virtual Base *unserialize(cereal::JSONInputArchive &ar, Entity e) = 0;
		virtual Base *unserialize(cereal::BinaryInputArchive &ar, Entity e) = 0;
		virtual Base *unserialize(cereal::XMLInputArchive &ar, Entity e) = 0;
		virtual Base *unserialize(cereal::PortableBinaryInputArchive &ar, Entity e) = 0;
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
	};

	template <class T>
	struct ComponentBase : public Base
	{
		ComponentBase()
			: Base(typeid(T).hash_code())
			//, _name(std::regex_replace(typeid(T).name(), std::regex("(\\s)+|(:)+"), "_"))
			, _name(typeid(T).name())
		{
		}

		virtual ~ComponentBase()
		{
		}

		static unsigned int getTypeId()
		{
			static unsigned int id = uniqueId();
			return id;
		}

		virtual Base *unserialize(cereal::JSONInputArchive &ar, Entity e)
		{
			return dynamic_cast<T*>(this)->unserialize<cereal::JSONInputArchive>(ar, e);
		}

		virtual Base *unserialize(cereal::BinaryInputArchive &ar, Entity e)
		{
			return dynamic_cast<T*>(this)->unserialize<cereal::BinaryInputArchive>(ar, e);
		}

		virtual Base *unserialize(cereal::XMLInputArchive &ar, Entity e)
		{
			return dynamic_cast<T*>(this)->unserialize<cereal::XMLInputArchive>(ar, e);
		}

		virtual Base *unserialize(cereal::PortableBinaryInputArchive &ar, Entity e)
		{
			return dynamic_cast<T*>(this)->unserialize<cereal::PortableBinaryInputArchive>(ar, e);
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
		//std::multiset<PubSubKey> _subscriptions; // subscriptions to local entity events
	};
}

#endif