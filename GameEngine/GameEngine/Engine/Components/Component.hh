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
		Base();
		virtual ~Base();
		virtual void reset(){};

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
			_serialize(ar);
		}

	protected:
		static unsigned short uniqueId()
		{
			static unsigned short id = 0;
			return id++;
		}
	};

	template <class T>
	struct ComponentBase : public Base
	{
		ComponentBase()
			: Base()
		{
		}

		virtual ~ComponentBase()
		{
		}

		ComponentBase(ComponentBase &&other)
			: Base()
		{
			entityId = std::move(other.entityId);
		}

		ComponentBase &operator=(ComponentBase &&o)
		{
			entityId = std::move(other.entityId);
		}

		static unsigned short getTypeId()
		{
			static unsigned short id = uniqueId();
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
			ar(cereal::make_nvp(name(), *dynamic_cast<T*>(this)));
		}

		virtual void _serialize(cereal::BinaryOutputArchive &ar)
		{
			ar(*dynamic_cast<T*>(this));
		}

		virtual void _serialize(cereal::XMLOutputArchive &ar)
		{
			ar(cereal::make_nvp(name(), *dynamic_cast<T*>(this)));
		}

		virtual void _serialize(cereal::PortableBinaryOutputArchive &ar)
		{
			ar(*dynamic_cast<T*>(this));
		}

		ENTITY_ID entityId;
	private:
		ComponentBase(ComponentBase &other);
		ComponentBase &operator=(ComponentBase const &o);

		static std::string &name()
		{
			static std::string _name = typeid(T).name();
			return _name;
		}
	protected:
		struct PostSerializationInterface
		{};

		// used if their is post serialization work to do (example, load mesh)
		static std::function<void(AScene *scene
			, Component::Base *component
			, const PostSerializationInterface *postDatas) > _postSerialization;
	};
}