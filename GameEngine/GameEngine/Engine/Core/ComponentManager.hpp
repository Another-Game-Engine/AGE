#pragma once

#include <vector>
#include <Components/Component.hh>
#include <Core/AScene.hh>

class AComponentManager
{
public:
	AComponentManager()
		: _reorder(false)
	{}
	virtual ~AComponentManager()
	{}

	inline bool getReorder() const { return _reorder; }
	inline void setReorder(bool v) { _reorder = v; }
	virtual void reorder() = 0;
protected:
	bool _reorder;
};

template <typename T>
class ComponentManager : public AComponentManager
{
public:
	ComponentManager(AScene *scene)
		: _scene(scene)
	{}

	virtual ~ComponentManager()
	{}

	inline unsigned short getTypeId() const
	{
		return T::getTypeId();
	}

	inline 	std::vector<std::size_t> &getComponentRefs()
	{
		return _componentsRefs;
	}

	inline 	std::vector<T> &getComponentArray()
	{
		return _components;
	}

	void clearComponents()
	{
		auto s = _componentsRefs.size();
		if (_componentsRefs.size() == 0)
			return;
		std::size_t i = 0;
		_freeSlot.clear();
		for (auto &d : _components)
		{
			_freeSlot.push_back(i++);
			d.reset();
		}
	}

	template<typename... Args>
	T *addComponent(Entity &entity, Args &&...args)
	{
		// get the component type ID
		std::size_t id = T::getTypeId();

		// if entity already have component, return it
		if (entity.hasComponent<T>())
		{
			return getComponent(entity);
		}

		T *component = nullptr;

		if (_freeSlot.size() != 0)
		{
			std::size_t position = _freeSlot.back();
			_freeSlot.pop_back();

			if (entity.componentsTable.size() <= id)
				entity.componentsTable.resize(id + 1, (std::size_t)(-1));
			entity.componentsTable[id] = position;
			entity.getCode().add(id + MAX_TAG_NUMBER);

			_componentsEntity[position].first = entity.getHandle().getId();

			//init component
			component = &_components[_componentsRefs[position]];
			component->init(std::forward<Args>(args)...);
			_scene->informFilters(true, id + MAX_TAG_NUMBER, std::move(entity.getHandle()));
		}
		else
		{
			auto position = _components.size();
			auto refPosition = _componentsRefs.size();
			_componentsRefs.push_back(position);
			if (_componentsEntity.size() <= position)
				_componentsEntity.resize(position + 1);
			_componentsEntity[position] = std::make_pair(entity.getHandle().getId(), refPosition);
			_components.emplace_back(T());

			auto &cptable = entity.componentsTable;
			if (entity.componentsTable.size() <= id)
				entity.componentsTable.resize(id + 1, (std::size_t)(-1));
			entity.componentsTable[id] = refPosition;
			entity.getCode().add(id + MAX_TAG_NUMBER);

			//init component
			component = &_components.back();
			component->init(std::forward<Args>(args)...);
			_scene->informFilters(true, id + MAX_TAG_NUMBER, std::move(entity.getHandle()));
		}
		_reorder = true;
		return component;
	}

	T *getComponent(Entity &e)
	{
		// get the component type ID
		unsigned short id = T::getTypeId();

		if (!e.hasComponent<T>())
			return nullptr;
		return &_components[_componentsRefs[e.componentsTable[id]]];
	}

	bool removeComponent(Entity &e)
	{
		// get the component type ID
		unsigned short id = T::getTypeId();
		auto compoPosition = e.componentsTable[id];

		if (!e.hasComponent<T>())
			return false;
		_freeSlot.push_back(compoPosition);
		e.componentsTable[id] = (std::size_t)(-1);

		e.unsetComponent(id);
		_scene->informFilters(false, id + MAX_TAG_NUMBER, std::move(e.getHandle()));
		_reorder = true;
		return true;
	}

	virtual void reorder()
	{
		if (!_reorder)
			return;
		auto id = T::getTypeId();
		if (_componentsEntity.size() <= 1)
			return;
		quickSort(0, _componentsEntity.size() - 1);
		this->_reorder = false;
	}
private:
	void quickSort(std::size_t top, std::size_t bottom)
	{
		std::size_t middle;
		if (top < bottom)
		{
			middle = partition(top, bottom);
			quickSort(top, middle);
			quickSort(middle + 1, bottom);
		}
	}

	std::size_t partition(std::size_t top, std::size_t bottom)
	{
		auto x = _componentsEntity[top].first;
		auto i = top - 1;
		auto j = bottom + 1;

		do
		{
			do
			{
				--j;
			} while (x < _componentsEntity[j].first);

			do
			{
				++i;
			} while (x > _componentsEntity[i].first);

			if (i < j)
			{
				auto tmp = _components[_componentsRefs[_componentsEntity[i].second]];
				_components[_componentsRefs[_componentsEntity[i].second]] = std::move(_components[_componentsRefs[_componentsEntity[j].second]]);
				_components[_componentsRefs[_componentsEntity[j].second]] = std::move(tmp);
				auto tmpIndex = _componentsRefs[_componentsEntity[i].second];
				_componentsRefs[_componentsEntity[i].second] = _componentsRefs[_componentsEntity[j].second];
				_componentsRefs[_componentsEntity[j].second] = tmpIndex;
				auto tmpEnt = _componentsEntity[i];
				_componentsEntity[i] = _componentsEntity[j];
				_componentsEntity[j] = tmpEnt;
			}
		} while (i < j);
		return j;
	}

	std::vector<T> _components;
	std::vector<std::size_t> _componentsRefs;
	std::vector<std::pair<std::size_t, std::size_t>> _componentsEntity;
	std::vector<std::size_t> _freeSlot;
	AScene *_scene;
};