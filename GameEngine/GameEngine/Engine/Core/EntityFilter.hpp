#pragma once

#include    <set>
#include	<Utils/Barcode.h>
#include    <Utils/PubSub.hpp>
#include    <Entities/EntityData.hh>
#include    <Core/AScene.hh>

bool defaultEntityComparaison(const Entity &e1, const Entity &e2);

class EntityFilter
{
public:
	EntityFilter(std::weak_ptr<AScene> scene, bool(*comparaisonFn)(const Entity&, const Entity&) = defaultEntityComparaison);
	virtual ~EntityFilter();

	template <typename T>
	void requireComponent()
	{
		auto id = T::getTypeId() + MAX_TAG_NUMBER;
		_code.add(id);
		_scene.lock()->filterSubscribe(id, this);
	}

	template <typename T>
	void unRequireComponent()
	{
		auto id = T::getTypeId() + MAX_TAG_NUMBER;
		_code.remove(id);
		_scene.lock()->filterUnsubscribe(id, this);
	}

	void requireTag(unsigned short tag);
	void unRequireTag(unsigned short tag);

	const Barcode &getCode() const;
	std::set<Entity, bool(*)(const Entity&, const Entity&)> const &getCollection();

	void virtual componentAdded(Entity &&e, unsigned short typeId);
	void virtual componentRemoved(Entity &&e, unsigned short typeId);

protected:
	std::set<Entity, bool(*)(const Entity&, const Entity&)> _collection;
	Barcode _code;
	std::weak_ptr<AScene> _scene;
};