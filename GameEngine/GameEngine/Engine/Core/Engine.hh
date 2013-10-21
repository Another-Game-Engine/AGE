
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

	Engine();
	Engine		&operator=(Engine const &oth);

public:
	Engine(IRenderContext *ctx);
	~Engine();

	void		addScene(AScene *scene, std::string const &name);
	void		removeScene(std::string const &name);
	void		bindScene(std::string const &name);

	Input						&inputs();
	Timer const					&timer();
	Resources::ResourceManager	&resources();
	Renderer					&renderer();

	bool 		start();
	bool 		update();
	void 		draw();
	void 		stop();
};

#include "Context/SdlContext.hh"

typedef Singleton<Engine, SdlContext> 	GameEngine;

#endif