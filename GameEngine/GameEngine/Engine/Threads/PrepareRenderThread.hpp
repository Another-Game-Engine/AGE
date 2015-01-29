#pragma once

#include "Thread.hpp"
#include "QueuePusher.hpp"
#include "QueueOwner.hpp"
#include <Core/RenderScene.hpp>
#include <vector>
#include <memory>
#include <thread>

namespace AGE
{
	class PrepareRenderThread : public Thread, public QueuePusher, public QueueOwner
	{
	public:
		virtual bool init();
		virtual bool release();
		bool update();
		virtual bool launch();
		virtual bool stop();

		void setCameraProjection(const glm::mat4& projection, const PrepareKey &key);
		PrepareKey addCamera();
		void updateGeometry(
			const PrepareKey &key
			, const std::vector<SubMeshInstance> &meshs);
		PrepareKey addMesh();
		PrepareKey addPointLight();
		void setPointLight(glm::vec3 const &color, glm::vec3 const &range, const PrepareKey &id);
	private:
		PrepareRenderThread();
		virtual ~PrepareRenderThread();
		PrepareRenderThread(const PrepareRenderThread &) = delete;
		PrepareRenderThread(PrepareRenderThread &&) = delete;
		PrepareRenderThread &operator=(const PrepareRenderThread &) = delete;
		PrepareRenderThread &operator=(PrepareRenderThread &&) = delete;

		void _createRenderScene(AScene *scene);
		void _destroyRenderScene(AGE::RenderScene *scene);
		RenderScene *_getRenderScene(const AScene *scene);

		AGE::Vector < std::unique_ptr<AGE::RenderScene> > _scenes;
		AGE::RenderScene *_activeScene;

		std::thread _threadHandle;
		std::atomic_bool _insideRun;
		bool _run;

		friend class ThreadManager;
	};
}