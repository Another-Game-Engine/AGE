
#ifndef ENGINE_HH_
#define ENGINE_HH_

#include <map>
#include <typeinfo>

#include "Timer.hh"
#include "Renderer.hh"
#include "AScene.hh"
#include "Context/SdlContext.hh"
#include "Utils/Singleton.hh"
#include "ResourceManager/ResourceManager.hh"

class Engine
{
private:
  	IRenderContext 					*_context;

	AScene							*_sceneBinded;
	std::map<std::string, AScene*>	_scenes;

	Engine(Engine const &);
	Engine &operator=(Engine const &);

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
// Dangerous use  of void*
// to enhance !
public:
	template <typename T, typename TypeSelector = T>
	T &getInstance()
	{
		size_t id = typeid(TypeSelector).hash_code();
		if (_instances.find(id) == std::end(_instances))
		{
			auto n = new T();
			_instances.insert(std::make_pair(id, n));
		}
		return *(static_cast<T*>(_instances[id]));
	}

private:
	std::map<size_t, void*> _instances;

};

#include "Context/SdlContext.hh"

typedef Singleton<Engine> 	GameEngine;

#endif