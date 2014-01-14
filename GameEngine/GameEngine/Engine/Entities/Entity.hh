#ifndef   __HANDLE_HH__
# define  __HANDLE_HH__

#include <Utils/MetaData.hpp>

class EntityData;
class AScene;

class Entity
{
public:
	META_DATA(Entity);
	Entity(unsigned int id = 0, AScene *manager = nullptr);
	~Entity();
	const unsigned int getId() const;
	EntityData *operator->();
	EntityData *get() const;
	bool operator<(const Entity &o) const;
	bool operator==(const Entity &o) const;
	Entity(const Entity &o);
	Entity &operator=(const Entity &o);
private:
	unsigned int _id;
	AScene *_manager;
};

#endif    //__HANDLE_HH__