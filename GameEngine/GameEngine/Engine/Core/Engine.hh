
#ifndef ENGINE_HH_
#define ENGINE_HH_

#include "Timer.hh"
#include "Renderer.hh"
#include "AScene.hh"
#include "Context/SdlContext.hh"
#include "Utils/Singleton.hh"
#include "ResourceManager/ResourceManager.hh"

#include "Utils/Any.hpp"


#include <map>
#include <typeinfo>

class Engine
{
private:
  	IRenderContext 					*_context;

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

	bool        init();
	bool 		start();
	bool 		update();
	void 		draw();
	void 		stop();

/////////////////////////////////////////////////////
// Dependencie injector implementation
public:
	template <typename T, typename TypeSelector = T>
	T &getInstance()
	{
		size_t id = typeid(TypeSelector).hash_code();
		if (_instances.find(id) == std::end(_instances))
		{
			std::shared_ptr<Any> n(new Any(T()));
			_instances.insert(std::make_pair(id, n));
			return n->get<T>();
		}
		return _instances[id]->get<T>();
	}

private:
	std::map<size_t, std::shared_ptr<Any> > _instances;

};

#include "Context/SdlContext.hh"

typedef Singleton<Engine> 	GameEngine;

#endif