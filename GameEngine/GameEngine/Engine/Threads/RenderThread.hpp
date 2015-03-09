#pragma once

#include "Thread.hpp"
#include "QueuePusher.hpp"
#include "QueueOwner.hpp"
#include <Utils/Containers/Vector.hpp>
#include <Render/GeometryManagement/Painting/PaintingManager.hh>
#include <Render/Pipelining/Pipelines/IRenderingPipeline.hh>
#include <memory>
#include <vector>
#include <Render/Properties/Properties.hh>
#include <Engine/SpacePartitioning/Ouptut/RenderCamera.hh>
#include <Render/PipelineTypes.hpp>

namespace AGE
{
	class SdlContext;
	class Input;
	class Engine;
	struct DrawableCollection;
	struct RenderCamera;

	typedef Properties Material;

	class RenderThread : public Thread, public QueueOwner
	{
	public:
		virtual bool init();
		virtual bool release();
		bool update();
		virtual bool launch();
		virtual bool stop();

		// used by render scene, maybe should be protected
		void createMeshProperty(const Key<Painter> &painter, Key<Properties> &properties, Key<Property> &transformation);

	public:
		std::vector<Material> _materials;
		std::shared_ptr<PaintingManager> paintingManager;
		std::vector<std::unique_ptr<IRenderingPipeline>> pipelines;

	private:

		void _recompileShaders();

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
		std::shared_ptr<RenderCameraListContainerHandle> _drawlistPtr;

		friend class ThreadManager;
	};
}