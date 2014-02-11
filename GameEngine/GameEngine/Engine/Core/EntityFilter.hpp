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
	void requireComponent()
	{
		auto id = T::getTypeId() + MAX_TAG_NUMBER;
		auto strId = std::to_string(id);
		_code.add(id);
		globalSub(std::string("componentAdded" + strId), [&](Entity entity){
			_componentAdded(entity, id);
		});
		globalSub(std::string("componentRemoved" + strId), [&](Entity entity){
			_componentRemoved(entity, id);
		});
	}

	template <typename T>
	void unRequireComponent()
	{
		auto id = T::getTypeId() + MAX_TAG_NUMBER;
		auto strId = std::to_string(id);

		_code.remove(id);
		unsub(std::string("componentAdded" + strId));
		unsub(std::string("componentRemoved" + strId));
	}

	void requireTag(unsigned int tag);
	void unRequireTag(unsigned int tag);

	const Barcode &getCode() const;
	std::set<Entity, bool(*)(Entity, Entity)> &getCollection();

protected:
	std::set<Entity, bool(*)(Entity, Entity)> _collection;
	Barcode _code;
	AScene *_scene;

	void _componentAdded(Entity &e, unsigned int typeId);
	void _componentRemoved(Entity &e, unsigned int typeId);
};