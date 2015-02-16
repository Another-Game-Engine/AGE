#pragma once

#include "Thread.hpp"
#include "QueuePusher.hpp"
#include "QueueOwner.hpp"
#include <Utils/Containers/Vector.hpp>
#include <Render/GeometryManagement/Painting/PaintingManager.hh>
#include <Render/Pipelining/Pipelines/IRenderingPipeline.hh>
#include <Render/Properties/PropertyManager.hh>
#include <memory>
#include <vector>

namespace AGE
{
	class SdlContext;
	class Input;
	class Engine;
	struct DrawableCollection;
	struct RenderCamera;

	class RenderThread : public Thread, public QueueOwner
	{
	public:
		virtual bool init();
		virtual bool release();
		bool update();
		virtual bool launch();
		virtual bool stop();

	public:
		std::shared_ptr<PaintingManager> paintingManager;
		std::vector<std::unique_ptr<IRenderingPipeline>> pipelines;
		std::shared_ptr<PropertyManager> properties;

	private:
		RenderThread();
		virtual ~RenderThread();
		RenderThread(const RenderThread &) = delete;
		RenderThread(RenderThread &&) = delete;
		RenderThread &operator=(const RenderThread &) = delete;
		RenderThread &operator=(RenderThread &&) = delete;

		std::thread _threadHandle;
		std::atomic_bool _insideRun;
		bool _run;

		SdlContext *_context;
		AGE::Vector<RenderCamera> _drawlist;

		friend class ThreadManager;
	};
}