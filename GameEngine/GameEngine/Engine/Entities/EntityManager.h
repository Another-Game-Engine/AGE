#ifndef   __ENTITY_MANAGER_H__
# define  __ENTITY_MANAGER_H__

#include <vector>
#include <queue>

class Engine;
class Handle;
class Entity;

class EntityManager
{
public:
	EntityManager(Engine *engine);
	virtual ~EntityManager();
	// IT'S IMPORTANT TO INIT ENTITY MANAGER !
	void init();
	Handle &createEntity();
	void destroy(const Handle &h);
	Entity *get(const Handle &h);
	Handle &getRoot();

private:
	Engine &_engine;
	Handle &_unique;
	std::vector<Entity> _pool;
	std::queue<unsigned int> _free;
};

#endif    //__ENTITY_MANAGER_H__