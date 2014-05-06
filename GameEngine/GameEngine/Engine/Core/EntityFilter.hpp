#pragma once

#include    <set>

#include    <Core/AScene.hh>

#include    <Entities/Entity.hh>

class EntityFilter
{
public:
	EntityFilter(std::weak_ptr<AScene> &&scene);
	virtual ~EntityFilter();

	template <typename T>
	void requireComponent()
	{
		auto id = T::getTypeId() + MAX_TAG_NUMBER;
		BitsetManipulation::set(_componentsBarcode, id);
		_scene.lock()->filterSubscribe(id, this);
	}

	template <typename T>
	void unRequireComponent()
	{
		auto id = T::getTypeId() + MAX_TAG_NUMBER;
		BitsetManipulation::unset(_componentsBarcode, id);
		_scene.lock()->filterUnsubscribe(id, this);
	}

	void requireTag(TAG_ID tag);
	void unRequireTag(TAG_ID tag);

	const std::unordered_set<ENTITY_ID> &getCollection() const;

	inline void clearCollection() { _collection.clear(); }

	void virtual componentAdded(Entity &&e, COMPONENT_ID typeId);
	void virtual componentRemoved(Entity &&e, COMPONENT_ID typeId);

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
	COMPONENTS_BARCODE _componentsBarcode;
	TAGS_BARCODE _tagsBarcode;
	std::unordered_set<ENTITY_ID> _collection;
	std::weak_ptr<AScene> _scene;

	void lock();
	void unlock();

private:
	bool _locked;
	std::unordered_set<ENTITY_ID> _trash;
};