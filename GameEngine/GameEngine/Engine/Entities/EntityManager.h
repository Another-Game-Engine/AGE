#ifndef   __ENTITY_MANAGER_H__
# define  __ENTITY_MANAGER_H__

#include <vector>
#include "Entity.hh"

class EntityManager
{
public:
	EntityManager();
	virtual ~EntityManager();
private:
	std::set<SmartPointer<Entity*> > _collection; // yeah I know set is not a good idea - it's just temporary
};

#endif    //__ENTITY_MANAGER_H__