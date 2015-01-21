#pragma once

#include <string>
#include <Entities/Entity.hh>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{
	class AScene;

	namespace	Component
	{
		struct Base
		{
			Base();
			virtual ~Base();
			virtual void reset(){};

			virtual void _unserialize(cereal::JSONInputArchive &ar, AScene *scene) = 0;
			virtual void _unserialize(cereal::BinaryInputArchive &ar, AScene *scene) = 0;
			virtual void _unserialize(cereal::XMLInputArchive &ar, AScene *scene) = 0;
			virtual void _unserialize(cereal::PortableBinaryInputArchive &ar, AScene *scene) = 0;
			virtual void _serialize(cereal::JSONOutputArchive &ar, AScene *scene) = 0;
			virtual void _serialize(cereal::BinaryOutputArchive &ar, AScene *scene) = 0;
			virtual void _serialize(cereal::XMLOutputArchive &ar, AScene *scene) = 0;
			virtual void _serialize(cereal::PortableBinaryOutputArchive &ar, AScene *scene) = 0;

			template <typename Archive>
			void serializeBase(Archive &ar, AScene *dependecyInjector)
			{
				_serialize(ar, dependecyInjector);
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

			virtual void _unserialize(cereal::JSONInputArchive &ar, AScene *scene)
			{
				ar(*(dynamic_cast<T*>(this)));
			}

			virtual void _unserialize(cereal::BinaryInputArchive &ar, AScene *scene)
			{
				ar(*(dynamic_cast<T*>(this)));
			}

			virtual void _unserialize(cereal::XMLInputArchive &ar, AScene *scene)
			{
				ar(*(dynamic_cast<T*>(this)));
			}

			virtual void _unserialize(cereal::PortableBinaryInputArchive &ar, AScene *scene)
			{
				ar(*(dynamic_cast<T*>(this)));
			}

			virtual void _serialize(cereal::JSONOutputArchive &ar, AScene *scene)
			{
				ar(cereal::make_nvp(name(), *dynamic_cast<T*>(this)));
			}

			virtual void _serialize(cereal::BinaryOutputArchive &ar, AScene *scene)
			{
				ar(*dynamic_cast<T*>(this));
			}

			virtual void _serialize(cereal::XMLOutputArchive &ar, AScene *scene)
			{
				ar(cereal::make_nvp(name(), *dynamic_cast<T*>(this)));
			}

			virtual void _serialize(cereal::PortableBinaryOutputArchive &ar, AScene *scene)
			{
				ar(*dynamic_cast<T*>(this));
			}

			static std::string &name()
			{
				static std::string _name = typeid(T).name();
				return _name;
			}

			static std::size_t &hash_code()
			{
				static std::size_t _name = typeid(T).hash_code();
				return _name;
			}

			ENTITY_ID entityId;
		private:
			ComponentBase(ComponentBase &other);
			ComponentBase &operator=(ComponentBase const &o);
			friend T;
		public:
			// used if their is post serialization work to do (example, load mesh)
			virtual void postUnserialization(AScene *scene)
			{}
		};
	}
}