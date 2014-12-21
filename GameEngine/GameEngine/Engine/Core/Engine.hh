#pragma once

#include <Configuration.hpp>
#include "Utils/DependenciesInjector.hpp"
#include <array>
#include <map>
#include <Core/SceneManager.hh>

namespace AGE
{
	class Timer;

	class Engine : public DependenciesInjector
#ifdef USE_DEFAULT_ENGINE_CONFIGURATION
		, public SceneManager
#endif
	{
	protected:
		Engine(Engine const &);
		Engine &operator=(Engine const &);

		Timer *_timer;
	public:
		Engine();
		virtual ~Engine();

		bool launch(std::function<bool()> &fn);
		bool update();
	};
}