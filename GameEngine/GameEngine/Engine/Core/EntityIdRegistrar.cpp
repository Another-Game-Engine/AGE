#include <Core/EntityIdRegistrar.hh>
#include <cassert>

EntityIdRegistrar::EntityIdRegistrar()
{}

EntityIdRegistrar::~EntityIdRegistrar()
{}


void EntityIdRegistrar::registrarUnserializedEntity(Entity e, std::size_t id)
{
	_unser.insert(std::make_pair(id, e));
//	updateEntityHandle(e, id);
}

std::size_t EntityIdRegistrar::registrarSerializedEntity(std::size_t id)
{
	static std::size_t counter = 0;
	if (_ser.find(id) != std::end(_ser))
		return _ser[id];
	_ser.insert(std::make_pair(id, counter));
	return counter++;
}

void EntityIdRegistrar::entityHandle(std::size_t id, Entity *e)
{
	_toUpdate.insert(std::make_pair(id, e));
}

void EntityIdRegistrar::updateEntityHandles()
{
	for (auto &e : _unser)
	{
		updateEntityHandle(e.second, e.first);
	}
	assert(_toUpdate.size() == 0 && "All handles have not been unserialized correctly.");
}

void EntityIdRegistrar::updateEntityHandle(Entity e, std::size_t id)
{
	auto ret = _toUpdate.equal_range(id);
	for (auto it = ret.first; it != ret.second; ++it)
	{
		*(it->second) = e;
	}
	_toUpdate.erase(id);
}
