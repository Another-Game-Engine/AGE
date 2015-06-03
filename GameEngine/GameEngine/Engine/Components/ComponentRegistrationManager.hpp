#pragma once

#include <map>
#include <functional>
#include <Components/Component.hh>
#include <Entities/EntityTypedef.hpp>

#define TYPE_TO_STRING(T)(#T);

namespace AGE
{
	class AScene;
	struct ComponentBase;

	class ComponentRegistrationManager
	{
		typedef std::function<void(ComponentBase *, cereal::JSONOutputArchive &)> RegisterJsonFn;
		typedef std::function<void(void *, cereal::JSONInputArchive &, Entity &)> LoadJsonFn;

		typedef std::function<void(AScene *)> CreateComponentPoolFn;
		typedef std::function<void(void *, ComponentBase *)> CopyFn;

		typedef std::function<void(ComponentBase *, cereal::PortableBinaryOutputArchive &)> RegisterBinaryFn;
		typedef std::function<void(void *, cereal::PortableBinaryInputArchive&, Entity &)> LoadBinaryFn;

	private:
		ComponentRegistrationManager();

	public:
		~ComponentRegistrationManager();
		static ComponentRegistrationManager &getInstance()
		{
			static ComponentRegistrationManager instance;
			return instance;
		}

		// This function is called at scene creation, so that pool are already created
		// and we can deserialize components without knowing their types
		void createComponentPool(AScene *scene)
		{
			for (auto &e : _createComponentPoolMap)
			{
				e.second(scene);
			}
		}

		template <class T>
		void registerComponentType(const char *name)
		{
			std::size_t key = T::getSerializableId();
			auto ageId = Component<T>::getTypeId();
			auto it = _creationFunctions.find(key);
			if (it != std::end(_creationFunctions))
				return;
			_creationFunctions.insert(std::make_pair(key, [](Entity *e){return e->addComponent<T>(); }));
			
			T tmpInstance;

			if (tmpInstance.isExposedInEditor())
			{
				_exposedTypes.insert(std::make_pair(key, ageId));
			}

			_typeIds.insert(std::make_pair(key, ageId));
			_ageTypeIds.insert(std::make_pair(ageId, key));

			_jsonSaveMap.insert(std::make_pair(ageId, RegisterJsonFn([=](ComponentBase *c, cereal::JSONOutputArchive &ar)
			{
				ar(cereal::make_nvp(T::getSerializableName(),*(static_cast<T*>(c))));
			})));

			_jsonLoadMap.insert(std::make_pair(ageId, LoadJsonFn([=](void *ptr, cereal::JSONInputArchive &ar, Entity &entity)
			{
				std::cout << "Loading component of type : " << name << std::endl;
				T *c = new(ptr)T();
				c->entity = entity;
				ar(*c);
			})));

			_binarySaveMap.insert(std::make_pair(ageId, RegisterBinaryFn([=](ComponentBase *c, cereal::PortableBinaryOutputArchive &ar)
			{
				ar(*(static_cast<T*>(c)));
			})));

			_binaryLoadMap.insert(std::make_pair(ageId, LoadBinaryFn([=](void *ptr, cereal::PortableBinaryInputArchive &ar, Entity &entity)
			{
				std::cout << "Loading component of type : " << name << std::endl;
				T *c = new(ptr)T();
				c->entity = entity;
				ar(*c);
			})));

			_createComponentPoolMap.insert(std::make_pair(ageId, CreateComponentPoolFn([](AScene *scene)
			{
				scene->createComponentPool<T>();
			})));

			_copyMap.insert(std::make_pair(ageId, CopyFn([](void *dest, ComponentBase *src)
			{
				T *c = new(dest)T();
			})));

			_componentNames.insert(std::make_pair(ageId, name));
		}

		const std::string &getComponentName(ComponentType type);
		void serializeJson(ComponentBase *c, cereal::JSONOutputArchive &ar);
		void serializeBinary(ComponentBase *c, cereal::PortableBinaryOutputArchive &ar);
		ComponentBase *loadJson(std::size_t componentHashId, Entity &e, cereal::JSONInputArchive &ar);
		ComponentBase *loadBinary(std::size_t componentHashId, Entity &e, cereal::PortableBinaryInputArchive &ar);
		std::size_t getSystemIdForAgeId(ComponentType id);
		ComponentBase *copyComponent(ComponentBase *c, AScene *scene);

		inline const std::map<std::size_t, std::function<ComponentBase*(Entity *)>> &getCreationFunctions() { return _creationFunctions; }
		inline const std::map<std::size_t, ComponentType> &getSystemIdToAgeIdMap() const { return _typeIds; }
		inline const std::map<std::size_t, ComponentType> &getExposedType() const { return _exposedTypes; }
		inline const std::map<ComponentType, std::size_t> &getAgeIdToSystemIdMap() const { return _ageTypeIds; }

	private:
		std::map<std::size_t, std::function<ComponentBase*(Entity *)>> _creationFunctions;
		std::map<std::size_t, ComponentType> _typeIds;
		// only exposed types in editor ( bool isExposedInEditor(){ return true; } )
		std::map<std::size_t, ComponentType> _exposedTypes;
		std::map<ComponentType, std::size_t> _ageTypeIds;
		std::map < ComponentType, RegisterJsonFn> _jsonSaveMap;
		std::map < ComponentType, LoadJsonFn> _jsonLoadMap;
		std::map < ComponentType, RegisterBinaryFn> _binarySaveMap;
		std::map < ComponentType, LoadBinaryFn> _binaryLoadMap;
		std::map < ComponentType, CopyFn> _copyMap;
		std::map < ComponentType, CreateComponentPoolFn> _createComponentPoolMap;
		std::map < ComponentType, std::string> _componentNames;
	};
}

#define REGISTER_COMPONENT_TYPE(T)(AGE::ComponentRegistrationManager::getInstance().registerComponentType<T>(#T));