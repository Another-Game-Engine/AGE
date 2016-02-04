#include "RenderThread.hpp"

#include <Core/Engine.hh>
#include <Core/Age_Imgui.hpp>
#include <Core/ConfigurationManager.hpp>

#include <Utils/ThreadName.hpp>
#include <Utils/OpenGL.hh>
#include <Utils/Memory.hpp>
#include <Utils/Debug.hpp>
#include <Utils/Profiler.hpp>
#include <Context/SDL/SdlContext.hh>

#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/ThreadManager.hpp>

#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Pipelining/Pipelines/CustomPipeline/DebugDeferredShading.hh>
#include <Render/GeometryManagement/SimpleGeometryManager.hpp>
#include <Render/GeometryManagement/DebugDrawManager.hpp>
#include <Render/PipelineTypes.hpp>
#include <Render/GeometryManagement/Painting/PaintingManager.hh>
#include <Render/OcclusionTools/DepthMapManager.hpp>
#include <Render/Pipelining/Pipelines/CustomPipeline/DeferredShading.hh>

// pour le hack deguelasse du bones texture
#include <Render\Pipelining\Pipelines\PipelineTools.hh>
#include <Render\Textures\TextureBuffer.hh>

#include <utility>

#include "Graphic/DRBCameraDrawableList.hpp"

namespace AGE
{
	RenderThread::RenderThread()
		: Thread(AGE::Thread::ThreadType::Render)
		, _context(nullptr),
		paintingManager(std::make_shared<PaintingManager>()),
		pipelines(RenderType::TOTAL),
		_depthMapManager(nullptr)
	{
#if defined(AGE_ENABLE_IMGUI)
		_imguiRenderlist = nullptr;
#endif
		_frameCounter = 0;
	}

	RenderThread::~RenderThread()
	{
	}

	void RenderThread::_recompileShaders()
	{
		// to be sure that this function is only called in render thread
		AGE_ASSERT(CurrentThread() == (AGE::Thread*)GetRenderThread());

		for (auto &e : pipelines)
		{
			if (!e)
			{
				continue;
			}
			e->recompileShaders();
		}
	}

	void RenderThread::_initPipelines()
	{
		// to be sure that this function is only called in render thread
		AGE_ASSERT(CurrentThread() == (AGE::Thread*)GetRenderThread());

		for (auto &e : pipelines)
		{
			if (!e)
			{
				continue;
			}
			e->init();
		}
	}

	void RenderThread::getCube(Key<Vertices> &v, Key<Painter> &p)
	{
		static const std::vector<glm::vec3> positions =
		{
			glm::vec3(-1.0f, 1.0f, 1.0f),
			glm::vec3(-1.0f, -1.0f, 1.0f),
			glm::vec3(1.0f, -1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f, 1.0f, -1.0f)
		};
		static const std::vector<unsigned int> indices =
		{
			0, 1, 2, 3, 0, 3, 7, 4, 4, 7, 6, 5, 6, 5, 1, 2, 2, 3, 7, 6, 4, 0, 1, 5
		};

		// to be sure that this function is only called in render thread
		AGE_ASSERT(CurrentThread() == (AGE::Thread*)GetRenderThread());

		if (SimpleGeometry::cubeMesh.verticesKey.isValid() &&
			SimpleGeometry::cubeMesh.painterKey.isValid())
		{
			v = SimpleGeometry::cubeMesh.verticesKey;
			p = SimpleGeometry::cubeMesh.painterKey;
			return;
		}

		auto type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC3, StringID("position", 0x4cbf3a26fca1d74a));
		std::vector<std::pair < GLenum, StringID > > types;
		types.push_back(type);

		if (!paintingManager->has_painter(types))
		{
			SimpleGeometry::cubeMesh.painterKey = paintingManager->add_painter(std::move(types));
		}
		else
		{
			SimpleGeometry::cubeMesh.painterKey = paintingManager->get_painter(types);
		}
		auto &painterPtr = paintingManager->get_painter(SimpleGeometry::cubeMesh.painterKey);

		SimpleGeometry::cubeMesh.verticesKey = painterPtr->add_vertices(positions.size(), indices.size());
		auto vertices = painterPtr->get_vertices(SimpleGeometry::cubeMesh.verticesKey);

		vertices->set_data<glm::vec3>(positions, StringID("position", 0x4cbf3a26fca1d74a));
		vertices->set_indices(indices);

		v = SimpleGeometry::cubeMesh.verticesKey;
		p = SimpleGeometry::cubeMesh.painterKey;
	}

	void RenderThread::getQuadGeometry(Key<Vertices> &v, Key<Painter> &p)
	{
		static const std::vector<glm::vec2> positions =
		{
			glm::vec2(-1.0f, 1.0f),
			glm::vec2(-1.0f, -1.0f),
			glm::vec2(1.0f, -1.0f),
			glm::vec2(1.0f, 1.0f)
		};
		static const std::vector<unsigned int> indices =
		{
			0, 1, 2, 0, 3, 2
		};

		// to be sure that this function is only called in render thread
		AGE_ASSERT(CurrentThread() == (AGE::Thread*)GetRenderThread());

		if (SimpleGeometry::quadMesh.verticesKey.isValid() &&
			SimpleGeometry::quadMesh.painterKey.isValid())
		{
			v = SimpleGeometry::quadMesh.verticesKey;
			p = SimpleGeometry::quadMesh.painterKey;
			return;
		}

		auto type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC2, StringID("position", 0x4cbf3a26fca1d74a));
		std::vector<std::pair < GLenum, StringID > > types;
		types.push_back(type);

		if (!paintingManager->has_painter(types))
		{
			SimpleGeometry::quadMesh.painterKey = paintingManager->add_painter(std::move(types));
		}
		else
		{
			SimpleGeometry::quadMesh.painterKey = paintingManager->get_painter(types);
		}
		auto &painterPtr = paintingManager->get_painter(SimpleGeometry::quadMesh.painterKey);

		SimpleGeometry::quadMesh.verticesKey = painterPtr->add_vertices(positions.size(), indices.size());
		auto vertices = painterPtr->get_vertices(SimpleGeometry::quadMesh.verticesKey);

		vertices->set_data<glm::vec2>(positions, StringID("position", 0x4cbf3a26fca1d74a));
		vertices->set_indices(indices);

		v = SimpleGeometry::quadMesh.verticesKey;
		p = SimpleGeometry::quadMesh.painterKey;
	}

	void RenderThread::getIcoSphereGeometry(Key<Vertices> &v, Key<Painter> &p, uint32_t recursion)
	{
		std::vector<glm::vec3> positions;
		std::vector<unsigned int> indices;

		// to be sure that this function is only called in render thread
		AGE_ASSERT(CurrentThread() == (AGE::Thread*)GetRenderThread());

		if (SimpleGeometry::icosphereMeshes[recursion].verticesKey.isValid() &&
			SimpleGeometry::icosphereMeshes[recursion].painterKey.isValid())
		{
			v = SimpleGeometry::icosphereMeshes[recursion].verticesKey;
			p = SimpleGeometry::icosphereMeshes[recursion].painterKey;
			return;
		}

		SimpleGeometry::generateIcoSphere(recursion, positions, indices);

		auto type = std::make_pair<GLenum, StringID>(GL_FLOAT_VEC3, StringID("position", 0x4cbf3a26fca1d74a));
		std::vector<std::pair < GLenum, StringID > > types;
		types.push_back(type);

		if (!paintingManager->has_painter(types))
		{
			SimpleGeometry::icosphereMeshes[recursion].painterKey = paintingManager->add_painter(std::move(types));
		}
		else
		{
			SimpleGeometry::icosphereMeshes[recursion].painterKey = paintingManager->get_painter(types);
		}
		auto &painterPtr = paintingManager->get_painter(SimpleGeometry::icosphereMeshes[recursion].painterKey);

		SimpleGeometry::icosphereMeshes[recursion].verticesKey = painterPtr->add_vertices(positions.size(), indices.size());
		auto vertices = painterPtr->get_vertices(SimpleGeometry::icosphereMeshes[recursion].verticesKey);

		vertices->set_data<glm::vec3>(positions, StringID("position", 0x4cbf3a26fca1d74a));
		vertices->set_indices(indices);

		v = SimpleGeometry::icosphereMeshes[recursion].verticesKey;
		p = SimpleGeometry::icosphereMeshes[recursion].painterKey;
	}

	bool RenderThread::init()
	{
		registerCallback<Tasks::Render::CreateRenderContext>([this](Tasks::Render::CreateRenderContext &msg)
		{
			_context = msg.engine->setInstance<SdlContext, IRenderContext>();

			auto configurationManager = msg.engine->getInstance<ConfigurationManager>();

			auto w = configurationManager->getConfiguration<int>("windowW")->getValue();
			auto h = configurationManager->getConfiguration<int>("windowH")->getValue();
			auto f = configurationManager->getConfiguration<bool>("fullScreen")->getValue();
			if (!_context->init(w, h, f, "~AGE~ V0.00001 Demo"))
			{
				msg.setValue(false);
				return;
			}

			if (_depthMapManager == nullptr)
			{
				_depthMapManager = new DepthMapManager;
			}

#ifdef DEBUG
			_depthMapManager->init(1280, 720, 4);
#else
			_depthMapManager->init(1280, 720, 2);
#endif

			msg.setValue(true);
		});

		registerCallback<Tasks::Render::InitRenderPipelines>([this](Tasks::Render::InitRenderPipelines &msg)
		{
			_context = msg.engine->setInstance<SdlContext, IRenderContext>();
			pipelines.resize(RenderType::TOTAL);
			pipelines[DEFERRED] = std::make_unique<DeferredShading>(_context->getScreenSize(), paintingManager);
			pipelines[DEBUG_DEFERRED] = std::make_unique<DebugDeferredShading>(_context->getScreenSize(), paintingManager);
			_recompileShaders();
			_initPipelines();
			_bonesTexture = createRenderPassOutput<TextureBuffer>(8184 * 2, GL_RGBA32F, sizeof(glm::mat4), GL_DYNAMIC_DRAW);
			msg.setValue(true);
		});

		registerCallback<Commands::ToRender::Flush>([&](Commands::ToRender::Flush& msg)
		{
			SCOPE_profile_cpu_i("RenderTimer", "Render frame");
			if (msg.isRenderFrame)
			{
#ifdef AGE_ENABLE_IMGUI
				std::shared_ptr<AGE::RenderImgui> imguiRenderList = nullptr;
				{
					std::lock_guard<AGE::SpinLock> lock(_mutex);

					imguiRenderList = _imguiRenderlist;
				}
				if (imguiRenderList != nullptr)
				{
					AGE::Imgui::getInstance()->renderThreadRenderFn(imguiRenderList->cmd_lists);
				}
				static bool first = true;
				if (first || imguiRenderList)
				{
					TMQ::TaskManager::emplaceMainTask<ImGuiEndOfFrame>();
					first = false;
				}
#endif
				_context->swapContext();
				{
					SCOPE_profile_gpu_i("Clear buffer");
					SCOPE_profile_cpu_i("RenderTimer", "Clear buffer");
					glClear(GL_COLOR_BUFFER_BIT);
				}
				++_frameCounter;

				if (_context)
				{
					_context->refreshInputs();
				}
				LMT_FLUSH();
			}
		});

		registerCallback<Tasks::Render::ReloadShaders>([&](Tasks::Render::ReloadShaders& msg)
		{
#ifdef AGE_DEBUG
			_recompileShaders();
#else
			std::cerr << "Error : You cannot recompile shader at runtime. This feature is enabled only in debug mode\n";
#endif
		});

 		registerSharedCallback<AGE::Tasks::Basic::BoolFunction>([&](AGE::Tasks::Basic::BoolFunction& msg)
		{
			SCOPE_profile_cpu_i("RenderTimer", "Bool function");
			msg.setValue(msg.function());
		});

		registerCallback<AGE::Tasks::Basic::VoidFunction>([&](AGE::Tasks::Basic::VoidFunction& msg)
		{
			SCOPE_profile_cpu_i("RenderTimer", "Void function");
			if (msg.function)
				msg.function();
		});

		registerCallback<AGE::Tasks::Basic::Exit>([&](AGE::Tasks::Basic::Exit& msg)
		{
			AGE::GetEngine()->deleteInstance<IRenderContext>();
			this->_insideRun = false;
			TMQ::TaskManager::emplaceSharedTask<Tasks::Basic::Exit>();
		});

		registerCallback<AGE::Tasks::Render::ContextGrabMouse>([&](AGE::Tasks::Render::ContextGrabMouse &msg)
		{
			_context->grabMouse(msg.grabMouse == 1 ? true : false);
		});

		registerCallback<AGE::DRBCameraDrawableListCommand>([&](AGE::DRBCameraDrawableListCommand &msg)
		{
			std::shared_ptr<Painter> line2DPainter = nullptr;
			std::shared_ptr<Painter> line3DPainter = nullptr;
			std::shared_ptr<Painter> line3DPainterDepth = nullptr;

			//if (pipelines[msg.list->cameraInfos.data.pipeline]->isDebug())
			//{
			DebugDrawManager* debugDrawManager = nullptr;
			if (GetEngine()->hasInstance<DebugDrawManager>())
			{
				debugDrawManager = GetEngine()->getInstance<DebugDrawManager>();
			}
			if (debugDrawManager)
			{
				debugDrawManager->renderBegin(paintingManager);
			}
			pipelines[msg.list->cameraInfos.data.pipeline]->render(*msg.list.get());
			if (debugDrawManager)
			{
				debugDrawManager->renderEnd();
			}
		});

		registerCallback<AGE::Tasks::UploadBonesToGPU>([&](AGE::Tasks::UploadBonesToGPU& msg)
		{
			SCOPE_profile_cpu_i("!!!HACK!!!", "Upload bones matrix to GPU");
			_bonesTexture->set(msg.bones->data(), msg.bones->size());
		});

		return true;
	}

	bool RenderThread::release()
	{
		if (_depthMapManager != nullptr)
			delete _depthMapManager;
		return true;
	}

	bool RenderThread::launch()
	{
		if (!init())
			return false;
		_threadHandle = std::thread(&RenderThread::update, std::ref(*this));
#ifdef AGE_ENABLE_PROFILING
		MicroProfileOnThreadExit();
#endif
		return true;
	}

	bool RenderThread::stop()
	{
		TMQ::TaskManager::emplaceRenderTask<Tasks::Basic::Exit>();
		if (_threadHandle.joinable())
			_threadHandle.join();
		return true;
	}

	bool RenderThread::update()
	{
		/*
		- Tant qu'il n'y a pas de command
		-> je pop des task
		- Une fois que j'ai mes commandes
		-> pour chacunes d'elles
		-> je regarde si c'est a moi de les traiter (ex : changement de scene)
		-> si ca n'est pas a moi de les traiter
		-> je les passe au render context actif
		-> si il n'y a pas de render context actif, c'est que j'ai fait une erreur, et j'assert dans ta face :D
		*/

		_registerId();

		_run = true;
		_insideRun = true;
		DWORD threadId = GetThreadId(static_cast<HANDLE>(_threadHandle.native_handle()));
		SetThreadName(threadId, this->_name.c_str());

		TMQ::MessageBase *task = nullptr;

		while (_run && _insideRun)
		{
			SCOPE_profile_cpu_i("RenderTimer", "Update");

			if (TMQ::TaskManager::RenderThreadGetTask(task))
			{
				SCOPE_profile_cpu_i("RenderTimer", "Execute task");
				auto success = execute(task); // we receive a task that we cannot treat
				AGE_ASSERT(success);
			}
		}
		return true;
	}

#ifdef AGE_ENABLE_IMGUI
	void RenderThread::setImguiDrawList(std::shared_ptr<AGE::RenderImgui> &list)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_imguiRenderlist = list;
	}
#endif
}
