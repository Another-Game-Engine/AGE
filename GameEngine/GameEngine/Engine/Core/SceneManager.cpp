#include "SceneManager.hh"
#include "AScene.hh"

SceneManager::SceneManager()
{}

SceneManager::~SceneManager()
{
	for (auto &e : _scenes)
		delete e.second;
	_scenes.clear();
}

void			SceneManager::addScene(AScene *scene, std::string const &name)
{
	_scenes[name] = scene;
}

void			SceneManager::removeScene(std::string const &name)
{
	_scenes.erase(name);
}

void			SceneManager::bindScene(std::string const &name)
{
	_sceneBinded = _scenes[name];
}

AScene			*SceneManager::getCurrentScene() const
{
	return (_sceneBinded);
}

bool            SceneManager::startScene() const
{
	if (!_sceneBinded)
		return false;
	return _sceneBinded->userStart();
}

bool            SceneManager::userUpdate(double time) const
{
	if (!_sceneBinded)
		return false;
	return _sceneBinded->userUpdate(time);
}

void            SceneManager::update(double time)
{
	if (_sceneBinded)
		_sceneBinded->update(time);
}