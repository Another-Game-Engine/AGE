#pragma once

#include "Utils/DependenciesInjector.hpp"

class Engine : public DependenciesInjector
{
private:
	Engine(Engine const &);
	Engine &operator=(Engine const &);

public:
	Engine();
	virtual ~Engine();

	bool        init(int mode, unsigned int swidth, unsigned int sheight, std::string &&name);
	bool 		start();
	bool 		update();
	void 		stop();
};