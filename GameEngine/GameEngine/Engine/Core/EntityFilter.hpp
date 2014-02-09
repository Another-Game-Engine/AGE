#pragma once

#include    <set>
#include	<Utils/Barcode.h>
#include    <Utils/PubSub.hpp>
#include    <Entities/EntityData.hh>
#include    <Core/AScene.hh>

bool defaultEntityComparaison(Entity e1, Entity e2);

class EntityFilter : public PubSub
{
public:
	EntityFilter(AScene *scene, bool(*comparaisonFn)(Entity, Entity) = defaultEntityComparaison);
	virtual ~EntityFilter();

	template <typename T>
	void require()
	{
		_code.add(T::getTypeId() + MAX_TAG_NUMBER);
		globalSub(std::string("componentAdded" + std::to_string(T::getTypeId())), [&](Entity entity){
			_componentAdded(entity, T::getTypeId());
		});
		globalSub(std::string("componentRemoved" + std::to_string(T::getTypeId())), [&](Entity entity){
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

	const Barcode &getCode() const;
	unsigned int getTag() const;
	void addTags(unsigned int tags);
	void removeTags(unsigned int tags);
	bool isTagged(unsigned int tags) const;
	std::set<Entity, bool(*)(Entity, Entity)> &getCollection();

protected:
	std::set<Entity, bool(*)(Entity, Entity)> _collection;
	Barcode _code;
	unsigned int _tags;
	AScene *_scene;

	void _componentAdded(Entity &e, unsigned int typeId);
	void _componentRemoved(Entity &e, unsigned int typeId);
};