#include <Core/EntityFilter.hpp>

bool defaultEntityComparaison(const Entity &e1, const Entity &e2)
{
	return e1 < e2;
}

EntityFilter::EntityFilter(std::weak_ptr<AScene> scene, bool(*comparaisonFn)(const Entity&, const Entity&))
: _collection(comparaisonFn)
, _scene(scene)
{
	assert(scene.lock() != nullptr && "System Scene is not valid.");
}

EntityFilter::~EntityFilter()
{
}

const Barcode &EntityFilter::getCode() const
{
	return _code;
}

std::set<Entity, bool(*)(const Entity&, const Entity&)> const &EntityFilter::getCollection()
{
	return _collection;
}

void EntityFilter::requireTag(unsigned short tag)
{
	_code.add(tag);
	_scene.lock()->filterSubscribe(tag, this);
}

void EntityFilter::unRequireTag(unsigned short tag)
{
	_code.remove(tag);
	_scene.lock()->filterUnsubscribe(tag, this);
}

void EntityFilter::componentAdded(Entity &&e, unsigned short typeId)
{
	if (_code.match(e->getCode()))
		_collection.insert(e);
}

void EntityFilter::componentRemoved(Entity &&e, unsigned short typeId)
{
	if (!_code.match(e->getCode()))
		_collection.erase(e);
}
