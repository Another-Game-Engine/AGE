#ifndef  __ENTITY_ID_REGISTRAR__
# define __ENTITY_ID_REGISTRAR__

#include <Entities/Entity.hh>
#include <map>

class EntityIdRegistrar
{
public:
	void registrarUnserializedEntity(Entity e, std::size_t id)
	{
		_unser.insert(std::make_pair(id, e));
		updateEntityHandle(e, id);
	}

	std::size_t registrarSerializedEntity(std::size_t id)
	{
		static std::size_t counter = 0;
		if (_ser.find(id) != std::end(_ser))
			return _ser[id];
		_ser.insert(std::make_pair(id, counter));
		return counter++;
	}

	void entityHandle(std::size_t id, Entity *e)
	{
		_toUpdate.insert(std::make_pair(id, e));
	}

	void updateEntityHandles()
	{
		for (auto &e : _unser)
		{
			updateEntityHandle(e.second, e.first);
		}
		assert(_toUpdate.size() == 0 && "All handles have not been unserialized correctly.");
	}
private:
	void updateEntityHandle(Entity e, std::size_t id)
	{
		auto ret = _toUpdate.equal_range(id);
		for (auto it = ret.first; it != ret.second; ++it)
		{
			*(it->second) = e;
		}
		_toUpdate.erase(id);
	}
private:
	std::map<std::size_t, Entity> _unser;
	std::map<std::size_t, std::size_t> _ser;
	std::multimap <std::size_t, Entity*> _toUpdate;
};

#endif   //__ENTITY_ID_REGISTRAR__