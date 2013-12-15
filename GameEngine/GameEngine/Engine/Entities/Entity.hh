
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
#include <Components/Component.hpp>

class Engine;
class EntityManager;
class Handle;

class Handle
{
public:
	Handle(unsigned int id = 0, EntityManager *manager = nullptr);
	~Handle();
	const unsigned int getId() const;
	Entity *operator->();
	Entity *get() const;
	bool operator<(const Handle &o) const;
	bool operator==(const Handle &o) const;
	Handle(const Handle &o);
	Handle &operator=(const Handle &o);
private:
	unsigned int _id;
	EntityManager *_manager;
};

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

	Handle   			_father;
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

	const Handle	    	&getFather() const;
	void 					setFather(Handle &father);

	void 					addSon(Handle &son);
	void 					removeSon(Handle &son);

	SmartPointer<t_EntityList> 	getSonsByFlags(size_t flags, GetFlags op);

	t_sonsList::iterator 		getSonsBegin();
	t_sonsList::iterator 		getSonsEnd();
	Barcode &getCode();


	////////////////////////////
	//
	//
	// COMPONENTS OPETATIONS
	//
	//
	//
	bool hasComponent(unsigned int componentId) const;
    #include "Entity.hpp"
};

#endif