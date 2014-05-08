#pragma once

#include <vector>
#include <Components/Component.hh>

class AScene;

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

	//inline 	std::vector<std::size_t> &getComponentRefs()
	//{
	//	return _componentsRefs;
	//}

	//inline 	std::vector<T> &getComponentArray()
	//{
	//	return _components;
	//}

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

		T *component = nullptr;

		if (_freeSlot.size() != 0)
		{
			std::size_t position = _freeSlot.back();
			_freeSlot.pop_back();
			_componentsEntity.emplace_back(std::make_pair(entity.getId(), position));
			_componentsRefs[entity.getId()] = position;

			//init component
			component = &_components[position];
			component->init(std::forward<Args>(args)...);
		}
		else
		{
			auto position = _components.size();

			_componentsRefs[entity.getId()] = position;
			_componentsEntity.emplace_back(std::make_pair(entity.getId(), position));
			_components.emplace_back(T());

			//init component
			component = &_components.back();
			component->init(std::forward<Args>(args)...);
		}
		_reorder = true;
		return component;
	}

	T *getComponent(const Entity &e)
	{
		return &_components[_componentsRefs[e.getId()]];
	}

	bool removeComponent(Entity &e)
	{
		_freeSlot.push_back(_componentsRefs[e.getId()]);
		_componentsEntity[_componentsRefs[e.getId()]] = _componentsEntity.back();
		_componentsEntity.pop_back();
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
	std::array<ENTITY_ID, MAX_ENTITY_NUMBER> _componentsRefs;
	std::vector<std::pair<ENTITY_ID, ENTITY_ID>> _componentsEntity;
	std::vector<ENTITY_ID> _freeSlot;
	AScene *_scene;
};