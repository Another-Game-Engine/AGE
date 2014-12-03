#pragma once

#include "Thread.hpp"
#include <Utils/Containers/Vector.hpp>
#include <memory>

namespace AGE
{
	class Engine;

	class MainThread : public Thread
	{
	public:
		virtual bool init();
		virtual bool release();
		bool update();
		AGE::Engine *createEngine();
		void destroyEngine(AGE::Engine *engine);
	private:
		MainThread();
		virtual ~MainThread();
		MainThread(const MainThread &) = delete;
		MainThread(MainThread &&) = delete;
		MainThread &operator=(const MainThread &) = delete;
		MainThread &operator=(MainThread &&) = delete;

		AGE::Vector < std::unique_ptr<AGE::Engine> > _engines;
		AGE::Engine *_activeEngine;
	};
}