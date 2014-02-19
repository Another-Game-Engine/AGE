#ifndef   __HANDLE_HH__
# define  __HANDLE_HH__

#include <Utils/MetaData.hpp>

class EntityData;
class AScene;

class Entity
{
public:
	Entity(unsigned int id = 0, AScene *manager = nullptr);
	~Entity();
	const unsigned int getId() const;
	const unsigned short getVersion() const;
	EntityData *operator->();
	EntityData *get() const;
	bool operator<(const Entity &o) const;
	bool operator==(const Entity &o) const;
	bool operator!=(const Entity &o) const;
	Entity(const Entity &o);
	Entity &operator=(const Entity &o);
private:
	friend class AScene;
	unsigned int _id;
	AScene *_manager;
	unsigned short _version;
};

#endif    //__HANDLE_HH__