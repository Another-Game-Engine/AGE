#pragma once

#include <Threads/CommandQueuePusher.hpp>
#include <Utils/Containers/Queue.hpp>
#include <Utils/Containers/Vector.hpp>

#include <Core/PrepareKey.hpp>

namespace AGE
{
	class Engine;

	class RenderContext : public CommandQueuePusher
	{
	public:
		RenderContext(Engine *engine) : _engine(engine) {}
		~RenderContext(){}
		RenderContext() = delete;
		RenderContext(const RenderContext &) = delete;
		RenderContext(RenderContext &&) = delete;
		RenderContext &operator=(const RenderContext &) = delete;
		RenderContext &operator=(RenderContext &&) = delete;
	private:
		Engine *_engine;
	};
}