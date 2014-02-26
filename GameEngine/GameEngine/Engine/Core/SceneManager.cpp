#include "SceneManager.hh"
#include "AScene.hh"

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
	// TODO
	// TO IMPLEMENT : CLEAR ENTITY POOL
	//	t->second->reset();
}

bool        SceneManager::initScene(std::string const &name)
{
	auto t = _scenes.find(name);
	if (t == std::end(_scenes))
		return false;
	return t->second->userStart();
}

bool            SceneManager::userUpdate(double time) const
{
	for (auto &e : _actives)
	{
		if (!e.second->userUpdate(time))
			return false;
	}
	return true;
}

void            SceneManager::update(double time)
{
	for (auto &e : _actives)
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		e.second->update(time);
	}
}