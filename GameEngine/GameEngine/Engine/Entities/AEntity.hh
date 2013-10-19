
#ifndef AEntity_HH_
#define AEntity_HH_

#include <map>

#include "Components/AComponent.hh"
#include "Core/Input.hh"
#include "Core/Timer.hh"
#include "Utils/SmartPointer.hh"
#include "OpenGL/Shader.hh"

#include "glm/glm.hpp"

class AEntity
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

	typedef std::map<size_t, SmartPointer<AEntity> >	t_sonsList;
	typedef std::list<SmartPointer<AEntity> >			t_EntityList;

private:
	size_t 				_id;
	size_t 				_flags;

	glm::mat4 			_localTransform;
	glm::mat4 			_globalTransform;

	AEntity 			*_father;
	t_sonsList 			_sons;

	std::map<std::string,
			SmartPointer<Components::AComponent> >		_components;

	AEntity(AEntity const &oth);
	AEntity 			&operator=(AEntity const &oth);

public:
	AEntity();
	virtual ~AEntity();

	glm::mat4 const  		&getLocalTransform();
	glm::mat4   			&setLocalTransform();

	glm::mat4 const			&getGlobalTransform() const;
	void 					computeGlobalTransform(glm::mat4 const &fatherTransform);

	virtual void 			draw() = 0;
	virtual void 			update(Timer const &timer, Input &input) = 0;

	size_t 					getId() const;

	size_t 					getFlags() const;
	void 					setFlags(size_t flags);
	void 					addFlags(size_t flags);
	void 					removeFlags(size_t flags);

	AEntity 				*getFather() const;
	void 					setFather(AEntity *father);

	void 					addSon(SmartPointer<AEntity> const &son);
	void 					removeSon(size_t sonId);
	SmartPointer<AEntity> 	getSon(size_t sonId);
	SmartPointer<AEntity> 	getSonRec(size_t sonId);

	SmartPointer<t_EntityList> 	getSonsByFlags(size_t flags, GetFlags op);

	void					addComponent(SmartPointer<Components::AComponent> const &component);
	bool					removeComponent(std::string const &name);

	t_sonsList::iterator 	getSonsBegin();
	t_sonsList::iterator 	getSonsEnd();
};

#endif