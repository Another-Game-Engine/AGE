
#ifndef ENGINE_HH_
#define ENGINE_HH_

#include "AScene.hh"
#include "Utils/DependenciesInjector.hpp"

class Engine : public DependenciesInjector
{
private:
	AScene							*_sceneBinded;
	std::map<std::string, AScene*>	_scenes;

	Engine(Engine const &);
	Engine &operator=(Engine const &);

	typedef	std::map<std::string, AScene*>::iterator	scenesIt;

public:
	Engine();
	virtual ~Engine();

	void		addScene(AScene *scene, std::string const &name);
	void		removeScene(std::string const &name);
	void		bindScene(std::string const &name);
	AScene		*getCurrentScene() const;

	bool        init();
	bool 		start();
	bool 		update();
	void 		stop();
};

#endif