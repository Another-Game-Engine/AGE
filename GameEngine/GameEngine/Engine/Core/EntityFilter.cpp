#include <Core/EntityFilter.hpp>

bool defaultEntityComparaison(Entity e1, Entity e2)
{
	return e1 < e2;
}

EntityFilter::EntityFilter(std::weak_ptr<AScene> scene, bool(*comparaisonFn)(Entity, Entity))
: PubSub(scene.lock()->getInstance<PubSub::Manager>())
, _collection(comparaisonFn)
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

std::set<Entity, bool(*)(Entity, Entity)> const &EntityFilter::getCollection()
{
	return _collection;
}

void EntityFilter::requireTag(unsigned int tag)
{
	auto strId = std::to_string(tag);

	_code.add(tag);
	globalSub(std::string("entityTagged" + strId), [&](Entity entity){
		_componentAdded(entity, tag);
	});
	globalSub(std::string("entityUntagged" + strId), [&](Entity entity){
		_componentRemoved(entity, tag);
	});
}

void EntityFilter::unRequireTag(unsigned int tag)
{
	auto strId = std::to_string(tag);

	_code.remove(tag);
	unsub(std::string("entityTagger" + strId));
	unsub(std::string("entityUntagged" + strId));
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
