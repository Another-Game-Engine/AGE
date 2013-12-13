#ifndef		__SYSTEM_H__
# define	__SYSTEM_H__

#include    <set>
#include	"Utils/Barcode.h"
#include    "Utils/PubSub.hpp"
#include    <Utils/SmartPointer.hh>
#include    <Entities/Entity.hh>

class Engine;

bool defaultEntityComparaison(Handle e1, Handle e2);

class	System : public PubSub
{
public:
	System(Engine &engine, bool(*)(Handle, Handle) = defaultEntityComparaison);
	virtual ~System();
	void update(double time);
	void init();
	const Barcode &getCode() const;

	template <typename T>
	void require()
	{
		_code.add<T>();
		sub(std::string("componentAdded" + std::to_string(T::getTypeId())), [&](Handle entity){
			_componentAdded(entity, T::getTypeId());
		});
		sub(std::string("componentRemoved" + std::to_string(T::getTypeId())), [&](Handle entity){
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
	std::set<Handle, bool(*)(Handle, Handle)> _collection;
	Barcode _code;

	virtual void _componentAdded(Handle e, unsigned int typeId)
	{
		if (_code.match(e->getCode()))
			_collection.insert(e);
	}

	virtual void _componentRemoved(Handle e, unsigned int typeId)
	{
		if (!_code.match(e->getCode()))
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