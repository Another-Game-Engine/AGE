#include "SceneManager.hh"
#include "AScene.hh"
#include <Threads/MainThread.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/Tasks/MainToPrepare.hpp>

namespace AGE
{
	SceneManager::SceneManager()
	{}

	SceneManager::~SceneManager()
	{
		_scenes.clear();
		_actives.clear();
	}

	void		SceneManager::addScene(std::shared_ptr<AScene> scene, std::string const &name)
	{

		GetPrepareThread()->getQueue()->emplaceFutureTask<AGE::Tasks::MainToPrepare::CreateScene, bool>(scene.get()).get();
		_scenes[name] = scene;

	}

	void		SceneManager::removeScene(std::string const &name)
	{
		auto t = _scenes.find(name);
		if (t == std::end(_scenes))
			return;
		for (auto &e = std::begin(_actives); e != std::end(_actives);)
		{
			auto a = e;
			++e;
			if (a->second == t->second)
			{
				_actives.erase(a);
				break;
			}
		}
		_scenes.erase(name);
	}

	void		SceneManager::enableScene(std::string const &name, unsigned int priority)
	{
		auto t = _scenes.find(name);
		if (t == std::end(_scenes))
			return;
		_actives[priority] = t->second;
	}

	void        SceneManager::disableScene(std::string const &name)
	{
		auto t = _scenes.find(name);
		if (t == std::end(_scenes))
			return;
		for (auto &e = std::begin(_actives); e != std::end(_actives);)
		{
			auto a = e;
			++e;
			if (a->second == t->second)
			{
				_actives.erase(a);
				break;
			}
		}
	}

	void        SceneManager::resetScene(std::string const &name)
	{
		auto t = _scenes.find(name);
		if (t == std::end(_scenes))
			return;
		t->second->clearAllEntities();
	}

	bool        SceneManager::initScene(std::string const &name)
	{
		auto t = _scenes.find(name);
		if (t == std::end(_scenes))
		{
			return false;
		}
		GetMainThread()->setSceneAsActive(t->second.get());
		GetPrepareThread()->getQueue()->emplaceCommand<Commands::MainToPrepare::SceneUpdateBegin>(t->second.get());
		return t->second->start();
	}

	bool            SceneManager::userUpdateScenes(double time) const
	{
		for (auto &e : _actives)
		{
			GetMainThread()->setSceneAsActive(e.second.get());
			GetPrepareThread()->getQueue()->emplaceCommand<Commands::MainToPrepare::SceneUpdateBegin>(e.second.get());
			if (!e.second->userUpdate(time))
				return false;
		}
		return true;
	}

	void            SceneManager::updateScenes(double time)
	{
		for (auto &e : _actives)
		{
			GetMainThread()->setSceneAsActive(e.second.get());
			GetPrepareThread()->getQueue()->emplaceCommand<Commands::MainToPrepare::SceneUpdateBegin>(e.second.get());
			e.second->update(time);
		}
	}
}