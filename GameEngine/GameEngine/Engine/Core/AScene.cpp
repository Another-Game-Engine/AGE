
#include <Core/Engine.hh>
#include <limits>
#include <Core/AScene.hh>
#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
#include <Entities/EntityFlags.hh>
#include <fstream>
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/PrepareRenderThread.hpp>

namespace AGE
{
	AScene::AScene(std::weak_ptr<AGE::Engine> engine) :
		DependenciesInjector(std::move(engine))
		, _entityNumber(0)
		, _engine(engine)
		, _renderScene(nullptr)
	{
		_componentsManagers.assign(nullptr);
	}

	AScene::~AScene()
	{
		_systems.clear();
	}

	void 							AScene::update(double time)
	{
		//static double reorderingTime = 0.0f;
		for (auto &e : _systems)
		{
			if (e.second->isActivated())
				e.second->update(time);
		}
		//reorderingTime += time;
		//if (reorderingTime > 0.4)
		//{
		//	reorganizeComponents();
		//	reorderingTime = 0;
		//}
	}

	bool                           AScene::start()
	{
		return userStart();
	}

	void                    AScene::registerFilter(EntityFilter *filter)
	{
		_allFilters.push_back(filter);
	}

	void                    AScene::filterSubscribe(COMPONENT_ID id, EntityFilter* filter)
	{
		auto findIter = std::find(_filters[id].begin(), _filters[id].end(), filter);
		if (findIter == std::end(_filters[id]))
			_filters[id].push_back(filter);
	}

	void                    AScene::filterUnsubscribe(COMPONENT_ID id, EntityFilter* filter)
	{
		_filters[id].remove(filter);
	}

	void                    AScene::informFiltersTagAddition(TAG_ID id, const EntityData &entity)
	{
		for (auto &&f : _filters[id])
		{
			f->tagAdded(entity, id);
		}
	}
	void                    AScene::informFiltersTagDeletion(TAG_ID id, const EntityData &entity)
	{
		for (auto &&f : _filters[id])
		{
			f->tagRemoved(entity, id);
		}
	}

	void                    AScene::informFiltersComponentAddition(COMPONENT_ID id, const EntityData &entity)
	{
		for (auto &&f : _filters[id])
		{
			f->componentAdded(entity, id);
		}
	}

	void                    AScene::informFiltersComponentDeletion(COMPONENT_ID id, const EntityData &entity)
	{
		for (auto &&f : _filters[id])
		{
			f->componentRemoved(entity, id);
		}
	}


	void                    AScene::informFiltersEntityCreation(const EntityData &entity)
	{
		for (auto &f : _allFilters)
		{
			f->entityAdded(entity);
		}
	}

	void                    AScene::informFiltersEntityDeletion(const EntityData &entity)
	{
		for (auto &f : _allFilters)
		{
			f->entityRemoved(entity);
		}
	}

	Entity &AScene::createEntity()
	{
		if (_free.empty())
		{
			auto &e = _pool[_entityNumber];
			e.entity.id = _entityNumber;
			e.link._octree = _renderScene;
			assert(++_entityNumber != UINT16_MAX);
			e.entity.setActive(true);
			informFiltersEntityCreation(e);
			return e.entity;
		}
		else
		{
			auto id = _free.front();
			_free.pop();
			_pool[id].link.reset();
			_pool[id].entity.setActive(true);
			informFiltersEntityCreation(_pool[id]);
			return _pool[id].entity;
		}
	}

	void AScene::destroy(const Entity &e)
	{
		Barcode cachedCode;
		auto &data = _pool[e.id];
		assert(data.entity == e);
		++data.entity.version;
		data.entity.flags = 0;
		data.entity.setActive(false);
		cachedCode = data.barcode;
		data.barcode.reset();
		for (std::size_t i = 0, mi = cachedCode.code.size(); i < mi; ++i)
		{
			if (i < MAX_CPT_NUMBER && cachedCode.code.test(i))
			{
				informFiltersComponentDeletion(COMPONENT_ID(i), data);
				_componentsManagers[i]->removeComponent(data.entity);
			}
			if (i >= MAX_CPT_NUMBER && cachedCode.code.test(i))
			{
				informFiltersTagDeletion(TAG_ID(i - MAX_CPT_NUMBER), data);
			}
		}
		informFiltersEntityDeletion(data);
		_free.push(e.id);
	}

	void AScene::clearAllEntities()
	{
		auto entityNbr = getNumberOfEntities();

		// we list entities
		auto ctr = 0;
		for (auto &e : _pool)
		{
			if (e.entity.isActive())
			{
				destroy(e.entity);
				//++ctr;
				//if (ctr >= entityNbr)
				//	break;
			}
		}
	}

	void AScene::saveToJson(const std::string &fileName)
	{
		std::ofstream file(fileName, std::ios::binary);
		assert(file.is_open());
		save<cereal::JSONOutputArchive>(file);
		file.close();
	}

	void AScene::loadFromJson(const std::string &fileName)
	{
		std::ifstream file(fileName, std::ios::binary);
		assert(file.is_open());
		load<cereal::JSONInputArchive>(file);
		file.close();
	}

	void AScene::saveToBinary(const std::string &fileName)
	{
		std::ofstream file(fileName, std::ios::binary);
		assert(file.is_open());
		save<cereal::BinaryOutputArchive>(file);
		file.close();
	}

	void AScene::loadFromBinary(const std::string &fileName)
	{
		std::ifstream file(fileName, std::ios::binary);
		assert(file.is_open());
		load<cereal::BinaryInputArchive>(file);
		file.close();
	}
	///////////////////////////////////////////////////////////

	void AScene::addTag(Entity &e, TAG_ID tag)
	{
		auto &data = _pool[e.id];
		if (data.entity != e)
			return;
		data.barcode.setTag(tag);
		informFiltersTagAddition(tag, data);
	}

	void AScene::removeTag(Entity &e, TAG_ID tag)
	{
		auto &data = _pool[e.id];
		if (data.entity != e)
			return;
		data.barcode.unsetTag(tag);
		informFiltersTagDeletion(tag, data);
	}

	bool AScene::isTagged(Entity &e, TAG_ID tag)
	{
		auto &data = _pool[e.id];
		if (data.entity != e)
			return false;
		return data.barcode.hasTag(tag);
	}

	Component::Base *AScene::getComponent(const Entity &entity, COMPONENT_ID componentId)
	{
		auto &e = _pool[entity.id];
		assert(e.entity == entity);
		assert(e.barcode.hasComponent(componentId));
		return this->_componentsManagers[componentId]->getComponentPtr(entity);
	}

	bool AScene::hasComponent(const Entity &entity, COMPONENT_ID componentId)
	{
		auto &e = _pool[entity.id];
		assert(e.entity == entity);
		return (e.barcode.hasComponent(componentId));
	}

	std::size_t AScene::getComponentHash(COMPONENT_ID componentId)
	{
		assert(this->_componentsManagers[componentId] != nullptr);
		return this->_componentsManagers[componentId]->getHashCode();
	}

	void AScene::reorganizeComponents()
	{
		for (auto &&e : _componentsManagers)
		{
			if (e != nullptr)
				e->reorder();
		}
	}

	const Entity *AScene::getEntityPtr(const Entity &e) const
	{
		auto &entity = _pool[e.id];
		if (entity.entity != e)
			return nullptr;
		return &(entity.entity);
	}

	const Entity &AScene::getEntityFromId(ENTITY_ID id) const
	{
		return _pool[id].entity;
	}

	AComponentManager *AScene::getComponentManager(COMPONENT_ID componentId)
	{
		return _componentsManagers[componentId];
	}

	AGE::Link *AScene::getLink(const Entity &e)
	{
		return &_pool[e.id].link;
	}

	AGE::Link *AScene::getLink(const ENTITY_ID &id)
	{
		return &_pool[id].link;
	}
}