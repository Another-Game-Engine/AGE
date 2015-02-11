#pragma once

#include <map>
#include <functional>
#include <Components/Component.hh>
#include <Entities/EntityTypedef.hpp>

namespace AGE
{
	class AScene;
	struct ComponentBase;

	class ComponentRegistrar
	{
		typedef std::function<void(ComponentBase *, cereal::JSONOutputArchive &)> RegisterJsonFn;
		typedef std::function<void(ComponentBase *, cereal::PortableBinaryOutputArchive &)> RegisterBinaryFn;

	private:
		ComponentRegistrar();

	public:
		~ComponentRegistrar();
		static ComponentRegistrar &getInstance()
		{
			static ComponentRegistrar instance;
			return instance;
		}

		std::size_t getHashCodeForAgeTypeId(ComponentType id)
		{
			auto f = _ageTypeIds.find(id);
			if (f == std::end(_ageTypeIds))
				return -1;
			return f->second;
		}

		template <class T>
		void registerComponentType()
		{
			std::size_t key = typeid(T).hash_code();
			auto ageId = Component<T>::getTypeId();
			auto it = _collection.find(key);
			if (it != std::end(_collection))
				return;
			_collection.insert(std::make_pair(key, [](){auto r = new T(); return r; }));
			_typeId.insert(std::make_pair(key, ageId));
			_ageTypeIds.insert(std::make_pair(ageId, key));

			_jsonSaveMap.insert(std::make_pair(ageId, RegisterJsonFn([](ComponentBase *c, cereal::JSONOutputArchive &ar)
			{
				ar(*(static_cast<T*>(c)));
			})));
			_binarySaveMap.insert(std::make_pair(ageId, RegisterBinaryFn([](ComponentBase *c, cereal::PortableBinaryOutputArchive &ar)
			{
				ar(*(static_cast<T*>(c)));
			})));
		}

		void serializeJson(ComponentBase *c, cereal::JSONOutputArchive &ar)
		{
			auto id = c->getType();
			auto find = _jsonSaveMap.find(id);
			assert(find != std::end(_jsonSaveMap));
			find->second(c, ar);
		}

		void serilizeBinary(ComponentBase *c, cereal::PortableBinaryOutputArchive &ar)
		{
			auto id = c->getType();
			auto find = _binarySaveMap.find(id);
			assert(find != std::end(_binarySaveMap));
			find->second(c, ar);
		}

		template <class Archive>
		ComponentBase *createComponentFromType(std::size_t type, Archive &ar, std::size_t &typeId, AScene *scene)
		{
			// @ECS TODO

			//auto &it = _collection.find(type);
			//auto &typeIt = _typeId.find(type);
			//assert((it != std::end(_collection) || typeIt != std::end(_typeId)) && "Component has not been registered");
			//auto res = (it->second)();
			//res->_unserialize(ar, scene);
			//typeId = typeIt->second;
			//return res;

			return nullptr;
		}

		std::map<std::size_t, std::function<ComponentBase*()>> _collection;
		std::map<std::size_t, std::size_t> _typeId;
		std::map<ComponentType, std::size_t> _ageTypeIds;
		std::map < ComponentType, RegisterJsonFn> _jsonSaveMap;
		std::map < ComponentType, RegisterBinaryFn> _binarySaveMap;
	};
}

#define REGISTER_COMPONENT_TYPE(T)(ComponentRegistrar::getInstance().registerComponentType<T>());