
#ifndef ENGINE_HH_
#define ENGINE_HH_

#include "AScene.hh"
#include "Utils/DependenciesInjector.hpp"

class Engine : public DependenciesInjector
{
private:
	Engine(Engine const &);
	Engine &operator=(Engine const &);

public:
	Engine();
	virtual ~Engine();

	bool        init();
	bool 		start();
	bool 		update();
	void 		stop();
};

#endif