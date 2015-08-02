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
#include <Culling/Ouptut/RenderCamera.hh>
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
	struct RenderImgui;

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
		void getCube(Key<Vertices> &vertices, Key<Painter> &painter);

		inline DepthMapManager &getDepthMapManager() { return _depthMapManager; }

		inline bool haveRenderFrameTask() const { return _haveRenderFrameTask; }
		void setCameraDrawList(std::shared_ptr<DRBCameraDrawableList> &list);
#ifdef AGE_ENABLE_IMGUI
		void setImguiDrawList(std::shared_ptr<AGE::RenderImgui> &list);
#endif

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
		std::atomic_bool _haveRenderFrameTask;
		std::shared_ptr<DRBCameraDrawableList> _cameraDrawableList;
#ifdef AGE_ENABLE_IMGUI
		std::shared_ptr<AGE::RenderImgui> _imguiRenderlist;
#endif
		bool _run;

		SdlContext *_context;
		std::list<std::shared_ptr<RenderCameraListContainerHandle>> _drawlists;
		DepthMapManager _depthMapManager;
		AGE::SpinLock _mutex;

		friend class ThreadManager;
	};
}