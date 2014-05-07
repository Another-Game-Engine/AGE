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
	BitsetManipulation::set(_tagsBarcode, id);
	_scene.lock()->filterSubscribe(id, this);
}

void EntityFilter::unRequireTag(TAG_ID id)
{
	BitsetManipulation::unset(_tagsBarcode, id);
	_scene.lock()->filterUnsubscribe(id, this);
}

void EntityFilter::componentAdded(Entity &&e, COMPONENT_ID typeId)
{
	if (BitsetManipulation::match(e.components, _componentsBarcode)
		&& BitsetManipulation::match(e.tags, _tagsBarcode))
	{
		_collection.insert(e.id);
	}
}

void EntityFilter::componentRemoved(Entity &&e, COMPONENT_ID typeId)
{
	if (BitsetManipulation::match(e.components, _componentsBarcode)
		&& BitsetManipulation::match(e.tags, _tagsBarcode))
	{
		if (!_locked)
		{
			_collection.erase(e.id);
		}
		else
		{
			_trash.insert(e.id);
		}
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
