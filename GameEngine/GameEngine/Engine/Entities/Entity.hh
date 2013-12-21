
#ifndef Entity_HH_
#define Entity_HH_

#include <map>

#include "Core/Input.hh"
#include "Core/Timer.hh"
#include "Utils/SmartPointer.hh"
#include "OpenGL/Shader.hh"
#include "Utils/Barcode.h"
#include "Utils/PubSub.hpp"
#include "glm/glm.hpp"
#include <Components/Component.hh>

class Engine;
class EntityManager;
class Handle;

class Entity : public PubSub
{
private:
	static size_t 					_currentId;
	Handle                          _handle;
public:
	enum	GetFlags
	{
		ONLY_THIS_FLAGS,
		THIS_FLAGS,
		NOT_THIS_FLAGS
	};

	enum	EntityFlags
	{
		HAS_MOVED = 1
	};

	typedef std::set<Handle>						t_sonsList;
	typedef std::list<Handle>						t_EntityList;
	typedef std::vector<SmartPointer<Component::Base> >             	t_ComponentsList;

	Handle &getHandle();
	void setHandle(Handle &handle);

private:
	Engine              &_engine;
	size_t 				_id;
	size_t 				_flags;

	glm::mat4 			_localTransform;
	glm::mat4 			_globalTransform;

	t_sonsList 			_sons;
	t_ComponentsList	_components;

	Barcode _code;
public:
	Entity(Engine &engine);
	virtual ~Entity();

	//Entity 			&operator=(Entity const &oth){}
	//Entity(Entity const &oth)
	//	: PubSub(oth._engine.getInstance<PubSub::Manager>()),
	//	_engine(oth._engine)
	//{
	//}

	glm::mat4 const  		&getLocalTransform();
	glm::mat4   			&setLocalTransform();

	glm::mat4 const			&getGlobalTransform() const;
	void 					computeGlobalTransform(glm::mat4 const &fatherTransform);

	size_t 					getId() const;

	size_t 					getFlags() const;
	void 					setFlags(size_t flags);
	void 					addFlags(size_t flags);
	void 					removeFlags(size_t flags);

	Barcode &getCode();


	////////////////////////////
	//
	//
	// COMPONENTS OPERATIONS
	//
	//
	//
	bool hasComponent(unsigned int componentId) const;

	template <typename T>
	bool hasComponent() const
	{
		return _code.isSet<T>();
	}

	template <typename T, typename... Args>
	SmartPointer<T> addComponent(Args ...args)
	{
		// get the component type ID
		unsigned int id = T::getTypeId();
		SmartPointer<T> res;

		// if entity already have component, return it
		if (hasComponent(id))
		{
			return static_cast<SmartPointer<T> >(_components[id]);
		}
		// else if entity components array is to small, resize it
		else if (_components.size() <= id)
		{
			_components.resize(id + 1);
		}
		// if component has never been created, create one
		if (!_components[id].get())
		{
			res = new T(_engine, getHandle());
			assert(res != nullptr && "Memory error : Component creation failed.");
			_components[id] = res;
		}
		else
			res = _components[id];
		//init component
		res->init(args...);
		_code.add(id);
		broadCast(std::string("componentAdded" + std::to_string(id)), _handle);
		return res;
	}

	template <typename T>
	SmartPointer<T> getComponent() const
	{
		unsigned int id = T::getTypeId();
		if (!hasComponent(id))
			return nullptr;
		return static_cast<SmartPointer<T> >(_components[id]);
	}

	template <typename T>
	void removeComponent()
	{
		unsigned int id = T::getTypeId();
		if (!hasComponent(id))
			return;
		_code.remove(id);
//		_components[id]	= nullptr;
		_components[id]->reset();
		broadCast(std::string("componentRemoved" + std::to_string(id)), _handle);
		// component remove -> signal to system
	}

};

#endif