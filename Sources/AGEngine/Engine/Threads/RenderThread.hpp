#pragma once

#include "Thread.hpp"
#include "QueuePusher.hpp"
#include "QueueOwner.hpp"
#include "Utils/Key.hh"

#include "glm/fwd.hpp"

#include <Utils/Containers/Vector.hpp>
#include <Utils/SpinLock.hpp>

#include <memory>
#include <vector>

namespace AGE
{
	class SdlContext;
	class Input;
	class Engine;
	struct DrawableCollection;
	struct RenderCamera;
	struct RenderImgui;
	class PaintingManager;
	class IRenderingPipeline;
	class DepthMapManager;
	class Painter;
	class IProperty;
	class Vertices;
	class TextureBuffer;

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
		std::vector<glm::vec2> debug2DlinesPoints;
		std::vector<glm::vec3> debug2DlinesColor;

		std::vector<glm::vec3> debug3DlinesPoints;
		std::vector<glm::vec3> debug3DlinesColor;
		// 3D lines with depth test
		std::vector<glm::vec3> debug3DlinesPointsDepth;
		std::vector<glm::vec3> debug3DlinesColorDepth;
		// Fill debug painters
		void fillDebugPainter(std::shared_ptr<Painter> &line2DPainter,
			std::shared_ptr<Painter> &line3DPainter,
			std::shared_ptr<Painter> &line3DPainterDepth);
		// End Debug drawing

		// used by render scene, maybe should be protected
		void getQuadGeometry(Key<Vertices> &vertices, Key<Painter> &painter);
		void getIcoSphereGeometry(Key<Vertices> &vertices, Key<Painter> &painter, uint32_t recursion);
		void getCube(Key<Vertices> &vertices, Key<Painter> &painter);

		inline std::size_t getCurrentFrameCount() const { return _frameCounter; }

		inline DepthMapManager &getDepthMapManager() { return *_depthMapManager; }

#ifdef AGE_ENABLE_IMGUI
		void setImguiDrawList(std::shared_ptr<AGE::RenderImgui> &list);
#endif
		std::shared_ptr<AGE::TextureBuffer> getBonesTexture() { return _bonesTexture; }
	public:
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

		std::atomic_bool _insideRun;

		std::size_t _frameCounter;

#ifdef AGE_ENABLE_IMGUI
		std::shared_ptr<AGE::RenderImgui> _imguiRenderlist;
#endif
		bool _run;

		SdlContext *_context;
		DepthMapManager *_depthMapManager;
		AGE::SpinLock _mutex;

		std::shared_ptr<AGE::TextureBuffer> _bonesTexture;

		friend class ThreadManager;
	};
}