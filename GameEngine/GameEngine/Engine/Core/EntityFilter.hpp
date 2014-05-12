#pragma once

#include    <set>

#include    <Core/AScene.hh>

#include    <Entities/Entity.hh>

#include <functional>

class EntityFilter;

class FilterFastCollection
{
public:
	void forEach(std::function<void()> &fn)
	{
		_it = 0;
		for (auto &e : _entities)
		{
			fn();
			++_it;
		}
	}

	const Entity &getEntity()
	{
		return *_entities[_it];
	}

	template <typename T>
	T *getComponent()
	{
		return static_cast<T*>(_datas[_typeRef[T::getTypeId()]][_it]);
	}

private:
	void build(const Barcode &code, std::set<Entity> &collection, std::weak_ptr<AScene> scene)
	{
		_scene = scene;
		_it = 0;
		if (!(code == _barcode))
		{
			buildCode(code);
		}
		buildDatas(collection);
	}

	void buildCode(const Barcode &code)
	{
		_barcode = code;
		_typeRef.clear();
		_types.clear();
		_typeRef.resize(MAX_CPT_NUMBER + MAX_TAG_NUMBER, std::size_t(MAX_ENTITY_NUMBER));

		_typeNumber = 0;
		for (auto i = 0; i < MAX_CPT_NUMBER; ++i)
		{
			if (i < MAX_CPT_NUMBER)
			{
				if (_barcode.hasComponent(i))
				{
					_typeRef[i] = _typeNumber++;
					_types.push_back(i);
				}
			}
		}
	}

	void buildDatas(std::set<Entity> &collection)
	{
		_datas.clear();

		auto scene = _scene.lock();

		_datas.resize(_typeNumber);
		std::size_t i = 0;
		for (auto t : _types)
		{
			auto mgr = scene->getComponentManager(t);
			for (auto e : collection)
			{
				_datas[i].push_back(mgr->getComponentPtr(e));
			}
			++i;
		}
		_entities.clear();
		for (auto e : collection)
		{
			_entities.push_back(scene->getEntityPtr(e));
		}
		_size = collection.size();
	}

	Barcode _barcode;
	std::weak_ptr<AScene> _scene;
	std::vector<std::size_t> _typeRef;
	std::vector<COMPONENT_ID> _types;
	std::size_t _typeNumber;
	std::vector<std::vector<void*>> _datas;
	std::vector<const Entity*> _entities;
	std::size_t _it;
	std::size_t _size;
	friend class EntityFilter;
};


class EntityFilter
{
public:
	EntityFilter(std::weak_ptr<AScene> &&scene);
	virtual ~EntityFilter();

	FilterFastCollection &createFilterFast()
	{
		_filterFast.build(_barcode, _collection, _scene);
		return _filterFast;
	}

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
	FilterFastCollection _filterFast;
};
