
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
#include <Components/ComponentRegistrar.hpp>

namespace AGE
{
	AScene::AScene(Engine *engine) :
		DependenciesInjector(std::move(engine))
		, _entityNumber(0)
		, _engine(engine)
		, _renderScene(nullptr)
	{
		ComponentRegistrar::getInstance().createComponentPool(this);
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

	void                    AScene::filterSubscribe(ComponentType id, EntityFilter* filter)
	{
		auto findIter = std::find(_filters[id].begin(), _filters[id].end(), filter);
		if (findIter == std::end(_filters[id]))
			_filters[id].push_back(filter);
	}

	void                    AScene::filterUnsubscribe(ComponentType id, EntityFilter* filter)
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

	void                    AScene::informFiltersComponentAddition(ComponentType id, const EntityData &entity)
	{
		for (auto &&f : _filters[id])
		{
			f->componentAdded(entity, id);
		}
	}

	void                    AScene::informFiltersComponentDeletion(ComponentType id, const EntityData &entity)
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
		auto e = _entityPool.create();

		if (_freeEntityId.empty())
		{
			e->entity.id = _entityNumber;
			++_entityNumber;
		}
		else
		{
			auto id = _freeEntityId.front();
			_freeEntityId.pop();
			e->entity.id = id;
		}
		e->link.reset();
		e->link._renderScene = _renderScene;
		e->entity.ptr = e;
		e->scene = this;
		informFiltersEntityCreation(*e);
		_entities.insert(e->entity);
		return e->entity;
	}

	void AScene::destroy(const Entity &e)
	{
		auto &data = e.ptr;
		auto find = _entities.find(e);
		assert(data->entity == e && find != std::end(_entities));
		_entities.erase(find);
		++data->entity.version;
		data->entity.flags = 0;
		_freeEntityId.push(e.id);
		for (ComponentType i = 0, mi = (ComponentType)data->components.size(); i < mi; ++i)
		{
			if (data->components[i])
			{
				informFiltersComponentDeletion(ComponentType(i), *data);
				data->removeComponent(i);
			}

			// @ECS TODO
			//if (i >= MAX_CPT_NUMBER && cachedCode.code.test(i))
			//{
			//	informFiltersTagDeletion(TAG_ID(i - MAX_CPT_NUMBER), *data);
			//}
		}
		informFiltersEntityDeletion(*data);
		_entityPool.destroy(e.ptr);
	}

	void AScene::clearAllEntities()
	{
		auto copy = _entities;
		for (auto &e : copy)
		{
			destroy(e);
		}
		_entities.clear();
	}

	void AScene::saveToJson(const std::string &fileName)
	{
		std::ofstream file(fileName, std::ios::binary);
		assert(file.is_open());

		{
			auto ar = cereal::JSONOutputArchive(file);

			std::size_t entityNbr = getNumberOfEntities();

			ar(cereal::make_nvp("Number_of_serialized_entities", entityNbr));

			auto &typesMap = ComponentRegistrar::getInstance().getAgeIdToSystemIdMap();
			ar(cereal::make_nvp("Component type map", typesMap));

			std::vector<EntitySerializationInfos> entities;
			for (auto &e : _entities)
			{
				EntitySerializationInfos es(e.ptr);
				for (auto &c : e.ptr->components)
				{
					if (c)
					{
						es.componentTypes.push_back(c->getType());
						es.components.push_back(c);
					}
				}
				ar(cereal::make_nvp("Entity_" + std::to_string(e.ptr->getEntity().getId()), es));
			}
		}
		file.close();
	}

	void AScene::loadFromJson(const std::string &fileName)
	{
		std::ifstream file(fileName, std::ios::binary);
		assert(file.is_open());

		{
			auto ar = cereal::JSONInputArchive(file);

			std::size_t entityNbr;
			ar(entityNbr);

			std::map<ComponentType, std::size_t> typesMap;
			ar(typesMap);

			for (std::size_t i = 0; i < entityNbr; ++i)
			{
				auto entity = createEntity();
				EntitySerializationInfos es(entity.ptr);
				es.typesMap = &typesMap;
				ar(es);
			}
		}
		file.close();
	}

	void AScene::saveToBinary(const std::string &fileName)
	{
		std::ofstream file(fileName, std::ios::binary);
		assert(file.is_open());

		file.close();
	}

	void AScene::loadFromBinary(const std::string &fileName)
	{
		std::ifstream file(fileName, std::ios::binary);
		assert(file.is_open());

		file.close();
	}
	///////////////////////////////////////////////////////////

	void AScene::addTag(Entity &e, TAG_ID tag)
	{
		// @ECS TODO

		//auto data = e.ptr;
		//if (data->entity != e)
		//	return;
		//data->barcode.setTag(tag);
		//informFiltersTagAddition(tag, *data);
	}

	void AScene::removeTag(Entity &e, TAG_ID tag)
	{
		// @ECS TODO

		//auto &data = e.ptr;
		//if (data->entity != e)
		//	return;
		//data->barcode.unsetTag(tag);
		//informFiltersTagDeletion(tag, *data);
	}

	bool AScene::isTagged(Entity &e, TAG_ID tag)
	{
		// @ECS TODO

		//auto &data = e.ptr;
		//if (data->entity != e)
		//	return false;
		//return data->barcode.hasTag(tag);
		return true;
	}
}