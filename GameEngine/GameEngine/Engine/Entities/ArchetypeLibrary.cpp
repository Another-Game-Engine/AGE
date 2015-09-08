#include "ArchetypeLibrary.hpp"

#include <FileUtils/FileUtils/FileSystemHelpers.hpp>
#include <Entities/Entity.hh>
#include <cereal/types/set.hpp>
#include <Utils/Path.hpp>
#include <Components/ArchetypeComponent.hpp>
#include <fstream>
#include <Core/AScene.hh>
#include <Entities/ArchetypeScenes.hpp>
#include <Core/SceneChunkSerialization.hpp>
#include <Entities/BinaryEntityPack.hpp>
#include <Entities/EntityBinaryPacker.hpp>

namespace AGE
{
	void ArchetypeLibrary::setPath(const std::string &library)
	{
		_libraryPath = library;
	}

	const std::string &ArchetypeLibrary::getPath() const
	{
		return _libraryPath;
	}

	void ArchetypeLibrary::load()
	{
		Directory dir;
		const bool success = dir.open(_libraryPath.c_str());
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

				ArchetypeFileModel model;
				BinaryEntityPack pack;
				pack.loadFromFile(*it);
				
				model.entity = pack.entities.front().entity;
				_library[model.name] = model;
			}
		}
		dir.close();
	}

	void ArchetypeLibrary::loadOne(const std::string &name)
	{
		if (_library.find(name) != std::end(_library))
		{
			return;
		}
		auto path = _libraryPath + "/" + name + ".archetype";

		ArchetypeFileModel model;

		BinaryEntityPack pack;
		pack.loadFromFile(path);

		model.entity = pack.entities.front().entity;

		_library[name] = model;
	}

	void ArchetypeLibrary::save()
	{
		for (auto &e : _library)
		{
			std::ofstream file(std::string(_libraryPath + "/" + e.second.name + ".archetype").c_str(), std::ios::binary);
			{
				auto ar = cereal::PortableBinaryOutputArchive(file);
				ar(e.second);
			}
			file.close();
		}
	}

	void ArchetypeLibrary::clear()
	{
		_library.clear();
	}

	void ArchetypeLibrary::addArchetype(const std::string &name, const Entity &entity)
	{
		_library[name] = ArchetypeFileModel();
		auto &e = _library[name];

		e.name = name;
		e.entity = entity;
	}

	std::shared_ptr<AScene> ArchetypeLibrary::getScene()
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

	void ArchetypeLibrary::enableScene()
	{
		GetEngine()->enableScene("ARCHETYPES_SCENE", 0);
	}

	void ArchetypeLibrary::disableScene()
	{
		GetEngine()->enableScene("ARCHETYPES_SCENE", 0);
	}

	// Entity needs to be created before ! (scene->createEntity())
	bool ArchetypeLibrary::spawn(const std::string &archetypeName, Entity &entity) const
	{
		auto it = _library.find(archetypeName);
		if (it == std::end(_library))
		{
			return false;
		}
		auto &entityRoot = it->second.entity;
		entity->getScene()->copyEntity(entityRoot, entity, true, false);
		entity->addComponent<AGE::ArchetypeComponent>(archetypeName);
		return true;
	}
}