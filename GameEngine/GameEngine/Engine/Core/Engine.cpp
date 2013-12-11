
#include "Engine.hh"
#include "Context/IRenderContext.hh"
#include "Utils/OpenGL.hh"
#include <iostream>

Engine::Engine() :
	_context(NULL),
	_sceneBinded(NULL)
{

	// DEFINE BASED DEPENDENCIES

	getInstance<Input>();
	getInstance<Timer>();
	getInstance<Resources::ResourceManager>();
//	getInstance<Renderer>();

 // 	IRenderContext 					*_context;

//	getInstance<IR>()

}

Engine::~Engine()
{
	if (_context != NULL)
		delete _context;
}

void			Engine::setContext(IRenderContext *ctx)
{
	if (_context != NULL)
		delete _context;
	_context = ctx;
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
	assert(_context != NULL && "Context must be initialized.");
	if (!_context->start(1920, 1080, "Mini solar system"))
		return (false);
 	if (glewInit() != GLEW_OK)
 	{
 		std::cerr << "glewInit Failed" << std::endl;
 		return (false);
 	}

	if (!renderer().init())
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	assert(_context != NULL && "Context must be initialized.");

	static auto &timer = getInstance<Timer>();
	static auto &inputs = getInstance<Input>();


	timer.update();
    inputs.clearInputs();
	_context->updateEvents(inputs);
	_sceneBinded->update(timer.getElapsed());

	_context->flush();

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
	assert(_context != NULL && "Context must be initialized.");

	static auto &renderer = Renderer();// getInstance<Renderer>();
	static auto &resources = getInstance<Resources::ResourceManager>();

	resources.uninit();
	renderer.uninit();
	for (scenesIt it = _scenes.begin(); it != _scenes.end(); ++it)
		delete it->second;
	_scenes.clear();
	_context->stop();
}
