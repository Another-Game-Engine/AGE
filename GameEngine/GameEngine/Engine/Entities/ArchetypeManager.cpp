#include "ArchetypeManager.hpp"
#include <Utils/Directory.hpp>
#include <FileUtils/FileUtils/FileSystemHelpers.hpp>
#include <Utils/Debug.hpp>
#include <Entities/BinaryEntityPack.hpp>
#include <Entities\ArchetypeScenes.hpp>
#include <Core/Engine.hh>
#include <Threads/MainThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/AScene.hh>

namespace AGE
{
	ArchetypeManager::ArchetypeManager()
	{
	}

	ArchetypeManager::~ArchetypeManager()
	{

	}

	void ArchetypeManager::load()
	{
		Directory dir;
		const bool success = dir.open(_libraryFolder.c_str());
		AGE_ASSERT(success);
		for (auto it = dir.recursive_begin(); it != dir.recursive_end(); ++it)
		{
			if (Directory::IsFile(*it))
			{
				auto extension = FileUtils::GetExtension(*it);

				if (extension != "archetype")
				{
					continue;
				}
				loadOne(FileUtils::RemoveExtension(FileUtils::GetName(*it)));
			}
		}
		dir.close();
	}

	void ArchetypeManager::loadOne(const std::string &name)
	{
		if (_archetypesCollection.find(name) != std::end(_archetypesCollection))
		{
			return;
		}

		auto currentScene = GetMainThread()->getActiveScene();

		auto path = _libraryFolder + "/" + name + ".archetype";
		BinaryEntityPack pack;
		pack.scene = getScene().get();

		pack.loadFromFile(path);

		auto &entity = pack.entities.front().entity;
		_archetypesCollection.insert(std::make_pair(name, entity));
	}

	void ArchetypeManager::spawn(Entity &entity, const std::string &name)
	{
		auto it = _archetypesCollection.find(name);
		if (it == std::end(_archetypesCollection))
		{
			load();
			it = _archetypesCollection.find(name);
		}
		AGE_ASSERT(it != std::end(_archetypesCollection));

		entity->getScene()->internalCopyEntity(it->second, entity, true, false);
		if (entity->haveComponent<AGE::ArchetypeComponent>())
		{
			entity->removeComponent<AGE::ArchetypeComponent>();
		}
		entity->addComponent<AGE::ArchetypeComponent>(name);
	}


	std::shared_ptr<AScene> ArchetypeManager::getScene()
	{
		if (_archetypesScene == nullptr)
		{
			_archetypesScene = std::make_shared<ArchetypeScene>(GetEngine());
			GetEngine()->addScene(_archetypesScene, "ARCHETYPES_SCENE");
			GetEngine()->initScene("ARCHETYPES_SCENE");
			GetEngine()->enableScene("ARCHETYPES_SCENE", 0);
		}
		return _archetypesScene;
	}

	void ArchetypeManager::enableScene()
	{
		GetEngine()->enableScene("ARCHETYPES_SCENE", 0);
	}

	void ArchetypeManager::disableScene()
	{
		GetEngine()->enableScene("ARCHETYPES_SCENE", 0);
	}

}