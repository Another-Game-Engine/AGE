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
#include <Render/OcclusionTools/DepthMapManager.hpp>
#include <Render/GeometryManagement/SimpleGeometry.hh>

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

		// temporary
		SimpleGeometry::SimpleGeometryKeys debug2Dlines;
		std::vector<glm::vec2> debug2DlinesPoints;

		// used by render scene, maybe should be protected
		void createMeshProperty(const Key<Painter> &painter, Key<Properties> &properties, Key<Property> &transformation);
		void getQuadGeometry(Key<Vertices> &vertices, Key<Painter> &painter);
		void getIcoSphereGeometry(Key<Vertices> &vertices, Key<Painter> &painter, uint32_t recursion);

		inline DepthMapManager &getDepthMapManager() { return _depthMapManager; }
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
		DepthMapManager _depthMapManager;

		friend class ThreadManager;
	};
}