#include <Core/EntityFilter.hpp>
#include <Core/AScene.hh>

namespace AGE
{
	EntityFilter::EntityFilter(AScene *scene)
		: _scene(scene)
		, _locked(false)
	{
		assert(_scene != nullptr && "System Scene is not valid.");
		_scene->registerFilter(this);
	}

	EntityFilter::~EntityFilter()
	{
	}

	std::set<Entity> &EntityFilter::getCollection()
	{
		return _collection;
	}

	const std::set<Entity> &EntityFilter::getCollection() const
	{
		return _collection;
	}

	void EntityFilter::requireComponent(ComponentType typeId)
	{
		_barcode.insert(typeId);
		_scene->filterSubscribe(typeId, this);
	}

	void EntityFilter::unRequireComponent(ComponentType typeId)
	{
		_barcode.erase(typeId);
		_scene->filterUnsubscribe(typeId, this);
	}

	void EntityFilter::requireTag(TAG_ID id)
	{
		// @ECS TODO

		//_barcode.setTag(id);
		//_scene.lock()->filterSubscribe(id, this);
	}

	void EntityFilter::unRequireTag(TAG_ID id)
	{
		// @ECS TODO

		//_barcode.unsetTag(id);
		//_scene.lock()->filterUnsubscribe(id, this);
	}

	bool EntityFilter::match(const EntityData &e)
	{
		for (auto &b : _barcode)
		{
			if (!e.haveComponent(b))
				return false;
		}
		return true;
	}

	void EntityFilter::componentAdded(const EntityData &e, ComponentType typeId)
	{

		if (match(e))
		{
			if (_locked)
			{
				_toAdd.insert(e.entity);
			}
			else
				_collection.insert(e.entity);
			if (_onAdd)
				_onAdd(e.entity);
		}
	}

	void EntityFilter::componentRemoved(const EntityData &e, ComponentType typeId)
	{
		if (!match(e))
		{
			if (_locked)
			{
				_trash.insert(e.entity);
			}
			else
				_collection.erase(e.entity);
			if (_onRemove)
				_onRemove(e.entity);
		}
	}

	void EntityFilter::tagAdded(const EntityData &e, TAG_ID typeId)
	{
		if (match(e))
		{
			if (_locked)
			{
				_toAdd.insert(e.entity);
			}
			else
				_collection.insert(e.entity);
			if (_onAdd)
				_onAdd(e.entity);
		}
	}

	void EntityFilter::tagRemoved(const EntityData &e, TAG_ID typeId)
	{
		if (!match(e))
		{
			if (_locked)
			{
				_trash.insert(e.entity);
			}
			else
				_collection.erase(e.entity);
			if (_onRemove)
				_onRemove(e.entity);
		}
	}

	void EntityFilter::entityAdded(const EntityData &e)
	{
		if (this->_barcode.empty())
		{
			if (_locked)
			{
				_toAdd.insert(e.entity);
			}
			else
				_collection.insert(e.entity);
			if (_onAdd)
				_onAdd(e.entity);
		}
	}

	void EntityFilter::entityRemoved(const EntityData &e)
	{
		if (_locked)
		{
			_trash.insert(e.entity);
		}
		else
			_collection.erase(e.entity);
		if (_onRemove)
			_onRemove(e.entity);
	}

	void EntityFilter::manuallyRemoveEntity(const Entity &e)
	{
		if (_locked)
		{
			_trash.insert(e);
		}
		else
			_collection.erase(e);
		if (_onRemove)
			_onRemove(e);
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
		for (auto &e : _toAdd)
		{
			_collection.insert(e);
		}
		for (auto &e : _trash)
		{
			_collection.erase(e);
		}
		_trash.clear();
		_toAdd.clear();
	}

	bool EntityFilter::isLocked() const
	{
		return _locked;
	}
}
