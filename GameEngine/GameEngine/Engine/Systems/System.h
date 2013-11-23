#ifndef		__SYSTEM_H__
# define	__SYSTEM_H__

#include    <unordered_set>
#include	"Utils/Barcode.h"
#include    "Utils/PubSub.hpp"
#include    <Utils/SmartPointer.hh>

class Entity;


class	System : public PubSub
{
public:
	System();
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
			_componentAdded<T>(entity);
		});
		sub(std::string("componentRemoved" + std::to_string(T::getTypeId())), [&](Entity *entity){
			_componentRemoved<T>(entity);
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
	std::unordered_set<Entity*> _collection;
	Barcode _code;

	template <typename T>
	void _componentAdded(Entity *e)
	{
		if (_code.match(*e))
			_collection.insert(e);
	}

	template <typename T>
	void _componentRemoved(Entity *e)
	{
		if (!_code.match(*e))
			_collection.erase(e);
	}

private:
	virtual void updateBegin(double time) = 0;
	virtual void updateEnd(double time) = 0;
	virtual void mainUpdate(double time) = 0;
	virtual void initialize() = 0;
};

#endif		//__SYSTEM_H__