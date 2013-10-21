
#include "Engine.hh"
#include "Context/IRenderContext.hh"
#include "Utils/OpenGL.hh"
#include <iostream>

Engine::Engine(IRenderContext *ctx) :
	_context(ctx),
	_sceneBinded(NULL)
{
}

Engine::~Engine()
{
	delete _context;
}

void			Engine::addScene(AScene *scene, std::string const &name)
{
	_scenes[name] = scene;
}

void			Engine::removeScene(std::string const &name)
{
	_scenes.erase(name);
}

void			Engine::bindScene(std::string const &name)
{
	_sceneBinded = _scenes[name];
}

Input			&Engine::inputs()
{
	return (_inputs);
}

Timer const		&Engine::timer()
{
	return (_timer);
}

Resources::ResourceManager &Engine::resources()
{
	return (_resources);
}

Renderer		&Engine::renderer()
{
	return (_renderer);
}

bool 		Engine::start()
{
	if (!_context->start(1280, 720, "super!"))
		return (false);
 	if (glewInit() != GLEW_OK)
 	{
 		std::cerr << "glewInit Failed" << std::endl;
 		return (false);
 	}
	if (!_sceneBinded || !_sceneBinded->userStart())
		return (false);
  	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return (true);
}

bool 		Engine::update()
{
	_timer.update();
	_context->updateEvents(_inputs);
	_sceneBinded->update();
	return (_sceneBinded->userUpdate());
}

void 		Engine::draw()
{
	_renderer.render();
	_context->flush();
}

void 		Engine::stop()
{
	_context->stop();
}
