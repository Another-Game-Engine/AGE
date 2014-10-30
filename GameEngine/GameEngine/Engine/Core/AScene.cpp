
#include <Core/Engine.hh>
#include <Utils/PubSub.hpp>
#include <limits>
#include <Core/AScene.hh>
#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
#include <Entities/EntityFlags.hh>
#include <Core/PrepareRenderThread.hpp>
#include <fstream>

AScene::AScene(std::weak_ptr<Engine> &&engine) :
DependenciesInjector(std::move(engine))
, _entityNumber(0)
{
	_componentsManagers.assign(nullptr);
}

AScene::~AScene()
{
	_systems.clear();
}

void 							AScene::update(double time)
{
	static double reorderingTime = 0.0f;
	for (auto &e : _systems)
	{
		if (e.second->isActivated())
			e.second->update(time);
	}
	reorderingTime += time;
	if (reorderingTime > 0.4)
	{
		reorganizeComponents();
		reorderingTime = 0;
	}
}

bool                           AScene::start()
{
	setInstance<PubSub::Manager>();
	return userStart();
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

Entity &AScene::createEntity()
	{
		if (_free.empty())
		{
			auto &e = _pool[_entityNumber];
			e.entity.id = _entityNumber;
			e.link._octree = getInstance<AGE::PrepareRenderThread>();
			assert(++_entityNumber != UINT16_MAX);
			e.entity.setActive(true);
			return e.entity;
		}
		else
		{
			auto id = _free.front();
			_free.pop();
			_pool[id].link.reset();
			_pool[id].entity.setActive(true);
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
	save<cereal::PortableBinaryOutputArchive>(file);
	file.close();
}