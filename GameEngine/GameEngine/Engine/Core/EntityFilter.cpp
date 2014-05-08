#include <Core/EntityFilter.hpp>

EntityFilter::EntityFilter(std::weak_ptr<AScene> &&scene)
: _scene(std::move(scene))
, _locked(false)
{
	assert(_scene.lock() != nullptr && "System Scene is not valid.");
}

EntityFilter::~EntityFilter()
{
}

const std::set<Entity> &EntityFilter::getCollection() const
{
	return _collection;
}

void EntityFilter::requireTag(TAG_ID id)
{
	_barcode.setTag(id);
	_scene.lock()->filterSubscribe(id, this);
}

void EntityFilter::unRequireTag(TAG_ID id)
{
	_barcode.unsetTag(id);
	_scene.lock()->filterUnsubscribe(id, this);
}

void EntityFilter::componentAdded(EntityData &&e, COMPONENT_ID typeId)
{
	if (e.barcode.match(_barcode))
	{
		_collection.insert(e.entity);
	}
}

void EntityFilter::componentRemoved(EntityData &&e, COMPONENT_ID typeId)
{
	if (!e.barcode.match(_barcode))
	{
		_collection.insert(e.entity);
	}
}

void EntityFilter::tagAdded(EntityData &&e, TAG_ID typeId)
{
	if (e.barcode.match(_barcode))
	{
		_collection.insert(e.entity);
	}
}

void EntityFilter::tagRemoved(EntityData &&e, TAG_ID typeId)
{
	if (!e.barcode.match(_barcode))
	{
		_collection.insert(e.entity);
	}
}

void EntityFilter::lock()
{
	if (_locked)
		return;
	_locked = true;
}

void EntityFilter::unlock()
{
	if (!_locked)
		return;
	_locked = false;
	for (auto &&e : _trash)
	{
		_collection.erase(e);
	}
	_trash.clear();
}

bool EntityFilter::isLocked() const
{
	return _locked;
}
