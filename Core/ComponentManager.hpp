//#pragma once
//
//#include <Utils/Containers/Vector.hpp>
//#include <Components/Component.hh>
//#include <utility>
//#include <cereal/cereal.hpp>
//
//namespace AGE
//{
//	class AScene;
//
//	class AComponentManager
//	{
//	public:
//		AComponentManager()
//			: _reorder(false)
//			, _componentHash(-1)
//		{}
//		virtual ~AComponentManager()
//		{}
//
//		virtual bool removeComponent(Entity &e) = 0;
//		virtual ComponentBase *getComponentPtr(const Entity &e) = 0;
//		virtual ComponentBase *getComponentPtr(ENTITY_ID e) = 0;
//
//		virtual void getDatabaseRegister(std::size_t &hash, ComponentType &id) const = 0;
//		virtual void addComponentPtr(const Entity &e, ComponentBase *ptr) = 0;
//
//		inline std::size_t getHashCode() const { return _componentHash; }
//	protected:
//		std::size_t _componentHash;
//	};
//
//	template <typename T>
//	class ComponentManager : public AComponentManager
//	{
//	public:
//		ComponentManager(AScene *scene)
//			: _scene(scene)
//			, _size(0)
//		{
//			_componentHash = typeid(T).hash_code();
//		}
//
//		virtual ~ComponentManager()
//		{}
//
//		inline ComponentType getTypeId() const
//		{
//			return Component<T>::getTypeId();
//		}
//
//		const AGE::Vector<T> &getComponents() const
//		{
//			return _components;
//		}
//
//		const std::size_t getSize() const
//		{
//			return _size;
//		}
//
//		void clearComponents()
//		{
//			for (auto &&e : _components)
//				e.reset(_scene);
//			_size = 0;
//		}
//
//		template<typename... Args>
//		T *addComponent(Entity &entity, Args &&...args)
//		{
//			T *component = nullptr;
//			if (_components.size() <= _size)
//				_components.resize(_size + 16);
//			_componentsRefs[entity.getId()] = ENTITY_ID(_size);
//			_components[_size] = std::move(T());
//			_components[_size].entity = entity;
//			component = &_components[_size];
//			++_size;
//			//init component
//			component->init(_scene, std::forward<Args>(args)...);
//			_reorder = true;
//			return component;
//		}
//
//		// used for unserialization
//		virtual void addComponentPtr(const Entity &entity, ComponentBase *component)
//		{
//			//@ECS TODO
//
//			//if (_components.size() <= _size)
//			//	_components.resize(_size + 16);
//			//_componentsRefs[entity.getId()] = ENTITY_ID(_size);
//			//_components[_size] = std::move(T(std::move(*(static_cast<T*>(component)))));
//			//_components[_size].entity = entity;
//			//_components[_size].postUnserialization(this->_scene);
//			//++_size;
//			//delete component;
//			//_reorder = true;
//		}
//
//		T *getComponent(const Entity &e)
//		{
//			return &_components[_componentsRefs[e.getId()]];
//		}
//
//		T *getComponent(ENTITY_ID e)
//		{
//			return &_components[_componentsRefs[e]];
//		}
//
//
//		virtual bool removeComponent(Entity &e)
//		{
//			auto id = _componentsRefs[e.getId()];
//			_components[id].reset(_scene);
//			if (_size > 0 && id < _size - 1)
//			{
//				_componentsRefs[_components[_size - 1].entity.getId()] = id;
//				std::swap(_components[id].entity, _components[_size - 1].entity);
//				std::swap(_components[id], _components[_size - 1]);
//			}
//			--_size;
//			_reorder = true;
//			return true;
//		}
//
//		virtual void reorder()
//		{
//			//if (!_reorder)
//			//	return;
//			//if (_size <= 1)
//			//	return;
//			//quicksort(0, int(_size - 1));
//			//this->_reorder = false;
//		}
//
//		virtual ComponentBase *getComponentPtr(const Entity &e)
//		{
//			return static_cast<ComponentBase*>(getComponent(e));
//		}
//
//		virtual ComponentBase *getComponentPtr(ENTITY_ID e)
//		{
//			return static_cast<ComponentBase*>(getComponent(e));
//		}
//
//		virtual void getDatabaseRegister(std::size_t &hash, ComponentType &id) const
//		{
//			hash = typeid(T).hash_code();
//			id = Component<T>::getTypeId();
//		}
//
//	private:
//
//		AScene *_scene;
//	};
//}