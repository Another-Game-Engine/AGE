
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

#include "glm/glm.hpp"

class Entity : public PubSub
{
private:
	static size_t 					_currentId;

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

private:
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
	Entity();
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
	const Barcode &getCode() const;
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
		SmartPointer<T> tmp(new T(args...));
		// todo assert if new T fail
		_code.add(id);
		_components[id] = tmp;
		tmp->setFather(this);
		pub("componentAdded" + _id, this);
		return tmp;
	}

	template <typename T>
	T *getComponent() const
	{
		unsigned int id = T::getTypeId();
		if (!hasComponent(id))
			return nullptr;
		return static_cast<T*>(_components[id]);
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
		pub("componentRemoved" + _id, this);
		// component remove -> signal to system
	}

};

#endif