#pragma once

#include    <set>

#include    <Core/AScene.hh>

#include    <Entities/Entity.hh>

#include <functional>

class EntityFilter
{
public:
	EntityFilter(std::weak_ptr<AScene> &&scene);
	virtual ~EntityFilter();

	std::function<void(std::weak_ptr<AScene>, Entity &&e)> entityAdded;
	std::function<void(std::weak_ptr<AScene>, Entity &&e)> entityRemoved;

	template <typename T>
	void requireComponent()
	{
		_barcode.setComponent(T::getTypeId());
		_scene.lock()->filterSubscribe(T::getTypeId(), this);
	}

	template <typename T>
	void unRequireComponent()
	{
		_barcode.unsetComponent(T::getTypeId());
		_scene.lock()->filterUnsubscribe(T::getTypeId(), this);
	}

	void requireTag(TAG_ID tag);
	void unRequireTag(TAG_ID tag);

	std::set<Entity> &getCollection();

	inline void clearCollection() { _collection.clear(); }

	void virtual componentAdded(EntityData &&e, COMPONENT_ID typeId);
	void virtual componentRemoved(EntityData &&e, COMPONENT_ID typeId);
	void virtual tagAdded(EntityData &&e, TAG_ID typeId);
	void virtual tagRemoved(EntityData &&e, TAG_ID typeId);

	bool isLocked() const;

	struct Lock
	{
		Lock(EntityFilter &filter)
			: _filter(filter)
		{
			_filter.lock();
		}

		~Lock()
		{
			release();
		}

		void release()
		{
			_filter.unlock();
		}

	private:
		EntityFilter &_filter;
	};

protected:
	Barcode _barcode;
	std::set<Entity> _collection;
	std::weak_ptr<AScene> _scene;

	void lock();
	void unlock();

private:
	bool _locked;
	std::set<Entity> _trash;
	std::set<Entity> _toAdd;
};
