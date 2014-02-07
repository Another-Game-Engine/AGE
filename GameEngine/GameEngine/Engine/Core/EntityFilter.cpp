#include <Core/EntityFilter.hpp>

bool defaultEntityComparaison(Entity e1, Entity e2)
{
	return e1 < e2;
}

EntityFilter::EntityFilter(AScene *scene, bool(*comparaisonFn)(Entity, Entity))
: PubSub(scene->getInstance<PubSub::Manager>())
, _collection(comparaisonFn)
, _scene(scene)
{
	assert(scene != nullptr && "System Scene is not valid.");
}

EntityFilter::~EntityFilter()
{
}

const Barcode &EntityFilter::getCode() const
{
	return _code;
}

std::set<Entity, bool(*)(Entity, Entity)> &EntityFilter::getCollection()
{
	return _collection;
}

void EntityFilter::_componentAdded(Entity &e, unsigned int typeId)
{
	if (_code.match(e->getCode()))
		_collection.insert(e);
}

void EntityFilter::_componentRemoved(Entity &e, unsigned int typeId)
{
	if (!_code.match(e->getCode()))
		_collection.erase(e);
}
