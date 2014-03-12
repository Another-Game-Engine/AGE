#ifndef   __HANDLE_HH__
# define  __HANDLE_HH__

#include <cstddef>

class EntityData;
class AScene;

class Entity
{
public:
	Entity(std::size_t id = 0, AScene *manager = nullptr);
	~Entity();
	Entity(Entity &&o);
	const std::size_t getId() const;
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
	std::size_t _id;
	AScene *_manager;
	unsigned short _version;
};

#endif    //__HANDLE_HH__