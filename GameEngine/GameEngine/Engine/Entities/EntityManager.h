#ifndef   __ENTITY_MANAGER_H__
# define  __ENTITY_MANAGER_H__

#include <vector>
#include <queue>
#include "Entity.hh"

class Engine;

class EntityManager
{
public:
	struct Handle
	{
		Handle(unsigned int id, EntityManager *manager)
		: _id(id), _manager(_manager)
		{}

		~Handle()
		{}

		const unsigned int getId() const
		{
			return _id;
		}

		Entity *operator->()
		{
			return _manager->get(*this);
		}
	private:
		unsigned int _id;
		EntityManager *_manager;
	};

public:
	EntityManager(Engine &engine) :
		_engine(engine)
	{}

	virtual ~EntityManager()
	{}

	Handle createEntity()
	{
		if (_free.empty())
		{
			for (auto i = 0; i < 16; ++i)
			{
				_pool.push_back(Entity(_engine));
				_pool.back().setHandle(Handle(_pool.size() - 1, this));
				_free.push(_pool.size() - 1);
			}
		}
		unsigned int index = _free.back();
		_free.pop();
		return _pool[index].getHandle();
	}

	void destroy(const Handle &h)
	{
		_free.push(h.getId());
	}

	Entity *get(const Handle &h)
	{
		if (h.getId() >= _pool.size())
			return nullptr;
		return &_pool[h.getId()];
	}

private:
	Engine &_engine;
	std::vector<Entity> _pool;
	std::queue<unsigned int> _free;
};

typedef EntityManager::Handle EntityHandle;

#endif    //__ENTITY_MANAGER_H__