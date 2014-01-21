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

		virtual Base *unserialize(cereal::JSONInputArchive &ar) = 0;
		virtual Base *unserialize(cereal::BinaryInputArchive &ar) = 0;
		virtual Base *unserialize(cereal::XMLInputArchive &ar) = 0;
		virtual Base *unserialize(cereal::PortableBinaryInputArchive &ar) = 0;
		virtual void _serialize(cereal::JSONOutputArchive &ar) = 0;
		virtual void _serialize(cereal::BinaryOutputArchive &ar) = 0;
		virtual void _serialize(cereal::XMLOutputArchive &ar) = 0;
		virtual void _serialize(cereal::PortableBinaryOutputArchive &ar) = 0;

		template <typename Archive>
		void serialize(Archive &ar)
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

		virtual Base *unserialize(cereal::JSONInputArchive &ar)
		{
			return dynamic_cast<T*>(this)->unserialize<cereal::JSONInputArchive>(ar);
		}

		virtual Base *unserialize(cereal::BinaryInputArchive &ar)
		{
			return dynamic_cast<T*>(this)->unserialize<cereal::BinaryInputArchive>(ar);
		}

		virtual Base *unserialize(cereal::XMLInputArchive &ar)
		{
			return dynamic_cast<T*>(this)->unserialize<cereal::XMLInputArchive>(ar);
		}

		virtual Base *unserialize(cereal::PortableBinaryInputArchive &ar)
		{
			return dynamic_cast<T*>(this)->unserialize<cereal::PortableBinaryInputArchive>(ar);
		}

		virtual void _serialize(cereal::JSONOutputArchive &ar)
		{
			ar(cereal::make_nvp(typeid(T).name(), *dynamic_cast<T*>(this)));
		}

		virtual void _serialize(cereal::BinaryOutputArchive &ar)
		{
			ar(cereal::make_nvp(typeid(T).name(), *dynamic_cast<T*>(this)));
		}

		virtual void _serialize(cereal::XMLOutputArchive &ar)
		{
			ar(cereal::make_nvp(typeid(T).name(), *dynamic_cast<T*>(this)));
		}

		virtual void _serialize(cereal::PortableBinaryOutputArchive &ar)
		{
			ar(cereal::make_nvp(typeid(T).name(), *dynamic_cast<T*>(this)));
		}

	private:
		//std::multiset<PubSubKey> _subscriptions; // subscriptions to local entity events
	};
}

#endif