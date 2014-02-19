#pragma once

#include <Entities/Entity.hh>
#include <map>

class EntityIdRegistrar
{
public:
	EntityIdRegistrar();
	virtual ~EntityIdRegistrar();
	void registrarUnserializedEntity(Entity e, std::size_t id);
	std::size_t registrarSerializedEntity(std::size_t id);
	void entityHandle(std::size_t id, Entity *e);
	void updateEntityHandles();
private:
	void updateEntityHandle(Entity e, std::size_t id);
private:
	std::map<std::size_t, Entity> _unser;
	std::map<std::size_t, std::size_t> _ser;
	std::multimap <std::size_t, Entity*> _toUpdate;
};