#ifndef		__SYSTEM_H__
# define	__SYSTEM_H__

#include    <set>
#include	"Utils/Barcode.h"
#include    "Utils/PubSub.hpp"
#include    <Utils/SmartPointer.hh>

class Entity;
class Engine;

bool defaultEntityComparaison(Entity *e1, Entity *e2);

class	System : public PubSub
{
public:
	System(Engine &engine, bool(*)(Entity*, Entity*) = defaultEntityComparaison);
	virtual ~System();
	void update(double time);
	void entityUpdated(Entity &entity);
	void init();
	const Barcode &getCode() const;

	template <typename T>
	void require()
	{
		_code.add<T>();
		sub(std::string("componentAdded" + std::to_string(T::getTypeId())), [&](Entity *entity){
			_componentAdded(entity, T::getTypeId());
		});
		sub(std::string("componentRemoved" + std::to_string(T::getTypeId())), [&](Entity *entity){
			_componentRemoved(entity, T::getTypeId());
		});
	}

	template <typename T>
	void unRequire()
	{
		_code.remove<T>();
		unsub(std::string("componentAdded" + std::to_string(T::getTypeId())));
		unsub(std::string("componentRemoved" + std::to_string(T::getTypeId())));
	}


protected:
	std::set<Entity*, bool(*)(Entity*, Entity*)> _collection;
	Barcode _code;

	virtual void _componentAdded(Entity *e, unsigned int typeId)
	{
		if (_code.match(*e))
			_collection.insert(e);
	}

	virtual void _componentRemoved(Entity *e, unsigned int typeId)
	{
		if (!_code.match(*e))
			_collection.erase(e);
	}

	Engine &_engine;

private:
	virtual void updateBegin(double time) = 0;
	virtual void updateEnd(double time) = 0;
	virtual void mainUpdate(double time) = 0;
	virtual void initialize() = 0;
};

#endif		//__SYSTEM_H__