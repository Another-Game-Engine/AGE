
#include "Engine.hh"
#include "Context/IRenderContext.hh"
#include "Utils/OpenGL.hh"
#include <iostream>

Engine::Engine() :
	_sceneBinded(NULL)
{

	// DEFINE BASED DEPENDENCIES

	setInstance<Input>();
	setInstance<Timer>();
	setInstance<Resources::ResourceManager>();
	setInstance<Renderer>();
}

Engine::~Engine()
{
	for (auto &e : _instances)
		delete e.second;
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

AScene			*Engine::getCurrentScene() const
{
	return (_sceneBinded);
}

bool        Engine::init()
{
	auto &context = getInstance<IRenderContext>();

	if (!context.start(1920, 1080, "Mini solar system"))
		return (false);
 	if (glewInit() != GLEW_OK)
 	{
 		std::cerr << "glewInit Failed" << std::endl;
 		return (false);
 	}

	if (!getInstance<Renderer>().init())
		return false;
  	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return true;
}

bool 		Engine::start()
{
	if (!_sceneBinded || !_sceneBinded->userStart())
		return (false);
	return (true);
}

bool 		Engine::update()
{
	auto &context = getInstance<IRenderContext>();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto &timer = getInstance<Timer>();
	auto &inputs = getInstance<Input>();

	timer.update();
    inputs.clearInputs();
	context.updateEvents(inputs);
	_sceneBinded->update(timer.getElapsed());

	context.flush();

	return (_sceneBinded->userUpdate());
}

void 		Engine::draw()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//assert(_context != NULL && "Context must be initialized.");
	//_renderer.render(_timer.getElapsed());
	//_context->flush();
}

void 		Engine::stop()
{
	static auto &renderer = getInstance<Renderer>();
	static auto &resources = getInstance<Resources::ResourceManager>();
	auto &context = getInstance<IRenderContext>();

	resources.uninit();
	renderer.uninit();
	for (scenesIt it = _scenes.begin(); it != _scenes.end(); ++it)
		delete it->second;
	_scenes.clear();
	context.stop();
}
