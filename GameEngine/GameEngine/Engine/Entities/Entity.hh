
#ifndef Entity_HH_
#define Entity_HH_

#include <map>

#include "Components/Component.hpp"
#include "Core/Input.hh"
#include "Core/Timer.hh"
#include "Utils/SmartPointer.hh"
#include "OpenGL/Shader.hh"
#include "Utils/Barcode.h"
#include "Utils/PubSub.hpp"
#include "EntityManager.h"

#include "glm/glm.hpp"

class Engine;

class Entity : public PubSub
{
private:
	static size_t 					_currentId;
	EntityHandle                    _handle;
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

	typedef std::map<size_t, SmartPointer<Entity> >						t_sonsList;
	typedef std::list<SmartPointer<Entity> >							t_EntityList;
	typedef std::vector<SmartPointer<Component::Base> >             	t_ComponentsList;

	EntityHandle &getHandle();
	void setHandle(EntityHandle &handle);

private:
	Engine              &_engine;
	size_t 				_id;
	size_t 				_flags;

	glm::mat4 			_localTransform;
	glm::mat4 			_globalTransform;

	Entity   			*_father;
	t_sonsList 			_sons;
	t_ComponentsList	_components;

	Entity(Entity const &oth);
	Entity 			&operator=(Entity const &oth);

	std::string _tag;
	std::string _layer;
	Barcode _code;

public:
	Entity(Engine &engine);
	virtual ~Entity();

	glm::mat4 const  		&getLocalTransform();
	glm::mat4   			&setLocalTransform();

	glm::mat4 const			&getGlobalTransform() const;
	void 					computeGlobalTransform(glm::mat4 const &fatherTransform);

	size_t 					getId() const;

	size_t 					getFlags() const;
	void 					setFlags(size_t flags);
	void 					addFlags(size_t flags);
	void 					removeFlags(size_t flags);

	Entity 			    	*getFather() const;
	void 					setFather(Entity *father);

	void 					addSon(SmartPointer<Entity> const &son);
	void 					removeSon(size_t sonId);
	SmartPointer<Entity> 	getSon(size_t sonId);
	SmartPointer<Entity> 	getSonRec(size_t sonId);

	SmartPointer<t_EntityList> 	getSonsByFlags(size_t flags, GetFlags op);

	t_sonsList::iterator 		getSonsBegin();
	t_sonsList::iterator 		getSonsEnd();
	Barcode &getCode();
	bool hasComponent(unsigned int componentId) const;


	template <typename T>
	bool hasComponent() const
	{
		return code_.isSet<T>();
	}

	template <typename T, typename... Args>
	SmartPointer<T> addComponent(Args ...args)
	{
		unsigned int id = T::getTypeId();
		if (hasComponent(id))
		{
			return static_cast<SmartPointer<T> >(_components[id]);
		}
		else if (_components.size() <= id)
		{
			_components.resize(id + 10);
		}
		SmartPointer<T> tmp(new T(_engine, args...));
		// todo assert if new T fail
		_code.add(id);
		_components[id] = tmp;
		tmp->setFather(this);
		pub(std::string("componentAdded" + std::to_string(id)), this);
		return tmp;
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
		code_.remove(id);
		delete _components[id];
		_components[id]	= nullptr;
		pub(std::string("componentRemoved" + std::to_string(id)), this);
		// component remove -> signal to system
	}

};

#endif