#pragma once

#include <vector>
#include <Components/Component.hh>
#include <utility>

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
	virtual bool removeComponent(Entity &e) = 0;
	virtual void *getComponentPtr(const Entity &e) = 0;
protected:
	bool _reorder;
};

template <typename T>
class ComponentManager : public AComponentManager
{
public:
	ComponentManager(AScene *scene)
		: _scene(scene)
		, _size(0)
	{}

	virtual ~ComponentManager()
	{}

	inline unsigned short getTypeId() const
	{
		return T::getTypeId();
	}

	const std::vector<T> &getComponents() const
	{
		return _components;
	}

	const std::size_t getSize() const
	{
		return _size;
	}

	void clearComponents()
	{
		for (auto &&e : _components)
			e.reset();
		_size = 0;
	}

	template<typename... Args>
	T *addComponent(Entity &entity, Args &&...args)
	{
		T *component = nullptr;
		if (_components.size() <= _size)
			_components.resize(_size + 16);
		_componentsRefs[entity.getId()] = COMPONENT_ID(_size);
		_components[_size] = std::move(T());
		_components[_size].entityId = entity.getId();
		component = &_components[_size];
		++_size;
		//init component
		component->init(std::forward<Args>(args)...);
		_reorder = true;
		return component;
	}

	T *getComponent(const Entity &e)
	{
		return &_components[_componentsRefs[e.getId()]];
	}

	virtual bool removeComponent(Entity &e)
	{
		auto id = _componentsRefs[e.getId()];
		_components[id].reset();
		if (_size > 0 && id < _size - 1)
		{
			_componentsRefs[_components[_size - 1].entityId] = id;
			std::swap(_components[id].entityId, _components[_size - 1].entityId);
			std::swap(_components[id], _components[_size - 1]);
		}
		--_size;
		_reorder = true;
		return true;
	}

	virtual void reorder()
	{
		if (!_reorder)
			return;
		if (_size <= 1)
			return;
		quicksort(0, int(_size - 1));
		this->_reorder = false;
	}

	virtual void *getComponentPtr(const Entity &e)
	{
		return static_cast<void*>(getComponent(e));
	}

private:
	// The partition function
	int partition(int p, int r)
	{
		int pivot = _components[r].entityId;

		while (p < r)
		{
			while (_components[p].entityId < pivot)
				p++;

			while (_components[r].entityId > pivot)
				r--;

			if (_components[p].entityId == _components[r].entityId)
				p++;
			else if (p < r)
			{
				std::swap(_componentsRefs[_components[p].entityId], _componentsRefs[_components[r].entityId]);
				std::swap(_components[p].entityId, _components[r].entityId);
				std::swap(_components[p], _components[r]);
				//int tmp = input[p];
				//input[p] = input[r];
				//input[r] = tmp;
			}
		}

		return r;
	}

	// The quicksort recursive function
	void quicksort(int p, int r)
	{
		if (p < r)
		{
			int j = partition(p, r);
			quicksort(p, j - 1);
			quicksort(j + 1, r);
		}
	}

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
		auto x = _components[top].entityId;
		auto i = top - 1;
		auto j = bottom + 1;

		do
		{
			do
			{
				--j;
			} while (x < _components[j].entityId);

			do
			{
				++i;
			} while (x > _components[i].entityId);

			if (i < j)
			{

				std::swap(_components[i], _components[j]);
				std::swap(_componentsRefs[_components[i].entityId], _componentsRefs[_components[j].entityId]);
			}
		} while (i < j);
		return j;
	}

	std::vector<T> _components;
	std::array<ENTITY_ID, MAX_ENTITY_NUMBER> _componentsRefs;
	std::size_t _size;
	AScene *_scene;
};