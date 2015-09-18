#include "SceneManager.hh"
#include "AScene.hh"
#include <Threads/MainThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <threads/Tasks/MainToPrepareTasks.hpp>

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
		_scenes[name] = scene;
		scene->setName(name);
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
		t->second->setActive(true);
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
				auto t = a->second;
				_actives.erase(a);
				t->setActive(false);
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
		return t->second->start();
	}

	bool            SceneManager::updateScenes(float time)
	{
		SCOPE_profile_cpu_function("Main thread");

		for (auto &e : _actives)
		{
			if (!e.second->userUpdateBegin(time))
				return false;
			e.second->update(time);
			if (!e.second->userUpdateEnd(time))
				return false;
		}
		return true;
	}

	void SceneManager::getSceneList(std::vector<std::string> &list) const
	{
		for (auto &e : _scenes)
		{
			list.push_back(e.first);
		}
	}

	std::shared_ptr<AScene> SceneManager::getScene(const std::string &name)
	{
		
		auto it = _scenes.find(name);
		if (it == std::end(_scenes))
		{
			return nullptr;
		}
		return it->second;
	}
}