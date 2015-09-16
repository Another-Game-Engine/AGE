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

		// Debug Drawings
		// 2D lines
		SimpleGeometry::SimpleGeometryKeys debug2Dlines;
		std::vector<glm::vec2> debug2DlinesPoints;
		std::vector<glm::vec3> debug2DlinesColor;
		// 3D lines without depth test
		SimpleGeometry::SimpleGeometryKeys debug3Dlines;
		std::vector<glm::vec3> debug3DlinesPoints;
		std::vector<glm::vec3> debug3DlinesColor;
		// 3D lines with depth test
		SimpleGeometry::SimpleGeometryKeys debug3DlinesDepth;
		std::vector<glm::vec3> debug3DlinesPointsDepth;
		std::vector<glm::vec3> debug3DlinesColorDepth;
		// Fill debug painters
		void fillDebugPainter(std::shared_ptr<Painter> &line2DPainter,
			std::shared_ptr<Painter> &line3DPainter,
			std::shared_ptr<Painter> &line3DPainterDepth);
		// End Debug drawing

		// used by render scene, maybe should be protected
		void createMeshProperty(const Key<Painter> &painter, Key<Properties> &properties, Key<Property> &transformation);
		void getQuadGeometry(Key<Vertices> &vertices, Key<Painter> &painter);
		void getIcoSphereGeometry(Key<Vertices> &vertices, Key<Painter> &painter, uint32_t recursion);
		void getCube(Key<Vertices> &vertices, Key<Painter> &painter);

		inline std::size_t getCurrentFrameCount() const { return _frameCounter; }

		inline DepthMapManager &getDepthMapManager() { return _depthMapManager; }

#ifdef AGE_ENABLE_IMGUI
		void setImguiDrawList(std::shared_ptr<AGE::RenderImgui> &list);
#endif

	public:
		std::vector<Material> _materials;
		std::shared_ptr<PaintingManager> paintingManager;
		std::vector<std::unique_ptr<IRenderingPipeline>> pipelines;

	private:

		void _recompileShaders();
		void _initPipelines();
		RenderThread();
		virtual ~RenderThread();
		RenderThread(const RenderThread &) = delete;
		RenderThread(RenderThread &&) = delete;
		RenderThread &operator=(const RenderThread &) = delete;
		RenderThread &operator=(RenderThread &&) = delete;

		std::thread _threadHandle;
		std::atomic_bool _insideRun;

		std::size_t _frameCounter;

#ifdef AGE_ENABLE_IMGUI
		std::shared_ptr<AGE::RenderImgui> _imguiRenderlist;
#endif
		bool _run;

		SdlContext *_context;
		DepthMapManager _depthMapManager;
		AGE::SpinLock _mutex;

		friend class ThreadManager;
	};
}