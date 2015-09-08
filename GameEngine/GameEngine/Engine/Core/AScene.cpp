#include <Core/Engine.hh>
#include <limits>
#include <Core/AScene.hh>
#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
#include <Entities/EntityFlags.hh>
#include <fstream>
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Components/ComponentRegistrationManager.hpp>
#include <Physics/Fallback/NullPhysics.hpp>
#include <Utils/Profiler.hpp>
#include <Entities/BinaryEntityPack.hpp>
#include <Entities/EntityBinaryPacker.hpp>
#include <Core/Link.hpp>

#include <BFC/BFCLinkTracker.hpp>
#include <BFC/BFCBlockManagerFactory.hpp>
#include "Graphic/GraphicElementManager.hpp"
#include "Graphic/DRBLightElementManager.hpp"

namespace AGE
{
	AScene::AScene(Engine *engine) :
		DependenciesInjector(engine)
		, _entityNumber(0)
		, _engine(engine)
		, _rootLink(std::make_unique<Link>())
	{
#ifdef AGE_BFC
		_bfcLinkTracker = new BFCLinkTracker();
		_bfcBlockManagerFactory = new BFCBlockManagerFactory();
		setInstance<GraphicElementManager>(_bfcBlockManagerFactory);
		setInstance<DRBLightElementManager>(_bfcBlockManagerFactory);
#endif
	}

	AScene::~AScene()
	{
		clearAllEntities();
#ifdef AGE_BFC
		delete _bfcLinkTracker;
		delete _bfcBlockManagerFactory;
#endif
		_systems.clear();
	}

	void 							AScene::update(float time)
	{
		SCOPE_profile_cpu_function("Scenes");
		for (auto &e : _systems)
		{
			if (e.second->isActivated())
				e.second->update(time);
		}
	}

	bool                    AScene::userStart()
	{
		SCOPE_profile_cpu_function("Scenes");
		auto succes = _userStart();
		if (!succes)
		{
			return false;
		}
		ComponentRegistrationManager::getInstance().createComponentPool(this);
		return true;
	}

	bool                    AScene::userUpdateBegin(float time)
	{
		SCOPE_profile_cpu_function("Scenes");
		return _userUpdateBegin(time);
	}

	bool                    AScene::userUpdateEnd(float time)
	{
		SCOPE_profile_cpu_function("Scenes");
		auto ret = _userUpdateEnd(time);
		return ret;
	}

	bool                           AScene::start()
	{
		SCOPE_profile_cpu_function("Scenes");
		return userStart();
	}

	void                    AScene::registerFilter(EntityFilter *filter)
	{
		SCOPE_profile_cpu_function("Scenes");
		_allFilters.push_back(filter);
	}

	void                    AScene::filterSubscribe(ComponentType id, EntityFilter* filter)
	{
		SCOPE_profile_cpu_function("Scenes");
		auto findIter = std::find(_filters[id].begin(), _filters[id].end(), filter);
		if (findIter == std::end(_filters[id]))
			_filters[id].push_back(filter);
	}

	void                    AScene::filterUnsubscribe(ComponentType id, EntityFilter* filter)
	{
		SCOPE_profile_cpu_function("Scenes");
		_filters[id].remove(filter);
	}

	void                    AScene::informFiltersTagAddition(TAG_ID id, const EntityData &entity)
	{
		SCOPE_profile_cpu_function("Scenes");
		for (auto &&f : _filters[id])
		{
			f->tagAdded(entity, id);
		}
	}
	void                    AScene::informFiltersTagDeletion(TAG_ID id, const EntityData &entity)
	{
		SCOPE_profile_cpu_function("Scenes");
		for (auto &&f : _filters[id])
		{
			f->tagRemoved(entity, id);
		}
	}

	void                    AScene::informFiltersComponentAddition(ComponentType id, const EntityData &entity)
	{
		SCOPE_profile_cpu_function("Scenes");
		for (auto &&f : _filters[id])
		{
			f->componentAdded(entity, id);
		}
	}

	void                    AScene::informFiltersComponentDeletion(ComponentType id, const EntityData &entity)
	{
		SCOPE_profile_cpu_function("Scenes");
		for (auto &&f : _filters[id])
		{
			f->componentRemoved(entity, id);
		}
	}


	void                    AScene::informFiltersEntityCreation(const EntityData &entity)
	{
		SCOPE_profile_cpu_function("Scenes");
		for (auto &f : _allFilters)
		{
			f->entityAdded(entity);
		}
	}

	void                    AScene::informFiltersEntityDeletion(const EntityData &entity)
	{
		SCOPE_profile_cpu_function("Scenes");
		for (auto &f : _allFilters)
		{
			f->entityRemoved(entity);
		}
	}

	Entity &AScene::createEntity(bool outContext /* = false */)
	{
		SCOPE_profile_cpu_function("Scenes");
		auto e = _entityPool.create(this);

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
		e->entity.ptr = e;
		e->outOfContext = outContext;
		if (!outContext)
		{
			informFiltersEntityCreation(*e);
			_entities.insert(e->entity);
		}
		return e->entity;
	}

	void AScene::destroy(const Entity &e, bool deep /*= false*/)
	{
		SCOPE_profile_cpu_function("Scenes");
		auto &data = e.ptr;
		auto find = _entities.find(e);
		AGE_ASSERT(data->entity == e);
		AGE_ASSERT(find != std::end(_entities));
		_entities.erase(find);
		++data->entity.version;
		data->entity.flags = 0;
		_freeEntityId.push(e.id);
		for (ComponentType i = 0, mi = (ComponentType)data->components.size(); i < mi; ++i)
		{
			if (data->components[i])
			{
				data->removeComponent(i);
			}
		}
		if (!data->outOfContext)
		{
			informFiltersEntityDeletion(*data);
		}

		auto children = e->getLink().getChildren();
		for (auto &c : children)
		{
			if (deep)
			{
				destroy(c->getEntity()->getEntity(), deep);
			}
			else
			{
				c->detachParent();
			}
		}
		data->getLink().detachParent();

		_entityPool.destroy(e.ptr);
	}

	bool AScene::copyEntity(const Entity &source, Entity &destination, bool deep /*= true*/, bool outContext /*= false*/)
	{
		SCOPE_profile_cpu_function("Scenes");
		if (!source.isValid())
		{
			return false;
		}
		if (!destination.isValid())
		{
			destination = createEntity(outContext);
			destination->getLink().setPosition(source->getLink().getPosition());
			destination->getLink().setOrientation(source->getLink().getOrientation());
			destination->getLink().setScale(source->getLink().getScale());
		}

		if (deep)
		{
			auto &link = source->getLink();
			for (auto &e : link.getChildren())
			{
				Entity tmp;
				if (!copyEntity(e->getEntity()->getEntity(), tmp, deep, outContext))
				{
					return false;
				}
				destination->getLink().attachChild(tmp.getLinkPtr());
			}
			for (auto e : source->getComponentList())
			{
				if (e != nullptr)
				{
					destination->copyComponent(e);
				}
			}
		}
		return true;
	}

	void AScene::clearAllEntities()
	{
		SCOPE_profile_cpu_function("Scenes");
		auto copy = _entities;
		for (auto &e : copy)
		{
			destroy(e);
		}
		_entities.clear();
	}

	Link *AScene::getRootLink()
	{
		return _rootLink.get();
	}

	void AScene::save(const std::string &fileName)
	{
		SCOPE_profile_cpu_function("Scenes");
		BinaryEntityPack pack;
		std::vector<Entity> vec;
		for (auto &e : _entities)
		{
			vec.push_back(e);
		}
		CreateBinaryEntityPack(pack, vec);
		pack.saveToFile(fileName);
	}

	void AScene::load(const std::string &fileName)
	{
		SCOPE_profile_cpu_function("Scenes");
		BinaryEntityPack pack;
		pack.scene = this;
		pack.loadFromFile(fileName);
	}

#ifdef AGE_BFC
	BFCLinkTracker *AScene::getBfcLinkTracker()
	{
		return _bfcLinkTracker;
	}

	BFCBlockManagerFactory *AScene::getBfcBlockManagerFactory()
	{
		return _bfcBlockManagerFactory;
	}
#endif

}