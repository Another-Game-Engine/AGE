
#ifndef ENGINE_HH_
#define ENGINE_HH_

#include "Timer.hh"
#include "Renderer.hh"
#include "AScene.hh"
#include "Context/SdlContext.hh"
#include "Utils/Singleton.hh"
#include "ResourceManager/ResourceManager.hh"


#include <map>

class Engine
{
private:
  	IRenderContext 					*_context;
	Input 							_inputs;
  	Timer 							_timer;
	Resources::ResourceManager		_resources;
	Renderer						_renderer;

	AScene							*_sceneBinded;
	std::map<std::string, AScene*>	_scenes;

	Engine(Engine const &);
	Engine		&operator=(Engine const &);

	typedef	std::map<std::string, AScene*>::iterator	scenesIt;

public:
	Engine();
	~Engine();

	void		setContext(IRenderContext *ctx);

	void		addScene(AScene *scene, std::string const &name);
	void		removeScene(std::string const &name);
	void		bindScene(std::string const &name);
	AScene		*getCurrentScene() const;

	Input						&inputs();
	Timer const					&timer();
	Resources::ResourceManager	&resources();
	Renderer					&renderer();

	bool        init();
	bool 		start();
	bool 		update();
	void 		draw();
	void 		stop();
};

#include "Context/SdlContext.hh"

typedef Singleton<Engine> 	GameEngine;

#endif