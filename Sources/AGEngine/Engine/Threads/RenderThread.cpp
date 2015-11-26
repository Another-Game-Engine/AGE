#include "RenderThread.hpp"

#include <Core/Engine.hh>
#include <Core/Age_Imgui.hpp>
#include <Core/ConfigurationManager.hpp>

#include <Utils/ThreadName.hpp>
#include <Utils/OpenGL.hh>

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

		auto type = std::make_pair<GLenum, std::string>(GL_FLOAT_VEC3, "position");
		std::vector<std::pair < GLenum, std::string > > types;
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

		vertices->set_data<glm::vec3>(positions, std::string("position"));
		vertices->set_indices(indices);

		v = SimpleGeometry::cubeMesh.verticesKey;
		p = SimpleGeometry::cubeMesh.painterKey;
	}

	void RenderThread::fillDebugPainter(std::shared_ptr<Painter> &line2DPainter,
										std::shared_ptr<Painter> &line3DPainter,
										std::shared_ptr<Painter> &line3DPainterDepth)
	{
		SCOPE_profile_cpu_i("RenderTimer", "RenderDebugLines");
		// 2D lines
		if (debug2DlinesPoints.size() != 0)
		{
			std::vector<unsigned int> indices;
			auto type = std::make_pair<GLenum, std::string>(GL_FLOAT_VEC2, "position");
			std::vector<std::pair < GLenum, std::string > > types;
			types.push_back(type);
			type = std::make_pair<GLenum, std::string>(GL_FLOAT_VEC3, "color");
			types.push_back(type);

			indices.resize(debug2DlinesPoints.size());
			for (int i = 0; i < debug2DlinesPoints.size(); ++i)
			{
				indices[i] = i;
			}
			if (!paintingManager->has_painter(types))
			{
				Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.painterKey = paintingManager->add_painter(std::move(types));
			}
			else
			{
				Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.painterKey = paintingManager->get_painter(types);
			}
			Key<Painter> kk = Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.painterKey;

			line2DPainter = paintingManager->get_painter(Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.painterKey);

			Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.verticesKey = line2DPainter->add_vertices(debug2DlinesPoints.size(), indices.size());
			auto vertices = line2DPainter->get_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.verticesKey);

			vertices->set_data<glm::vec2>(debug2DlinesPoints, std::string("position"));
			vertices->set_data<glm::vec3>(debug2DlinesColor, std::string("color"));
			vertices->set_indices(indices);

			debug2DlinesPoints.clear();
		}
		// 3D lines
		if (debug3DlinesPoints.size() != 0)
		{
			std::vector<unsigned int> indices;
			auto type = std::make_pair<GLenum, std::string>(GL_FLOAT_VEC3, "position");
			std::vector<std::pair < GLenum, std::string > > types;
			types.push_back(type);
			type = std::make_pair<GLenum, std::string>(GL_FLOAT_VEC3, "color");
			types.push_back(type);

			indices.resize(debug3DlinesPoints.size());
			for (int i = 0; i < debug3DlinesPoints.size(); ++i)
			{
				indices[i] = i;
			}
			if (!paintingManager->has_painter(types))
			{
				Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.painterKey = paintingManager->add_painter(std::move(types));
			}
			else
			{
				Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.painterKey = paintingManager->get_painter(types);
			}
			Key<Painter> kk = Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.painterKey;

			line3DPainter = paintingManager->get_painter(Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.painterKey);

			Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.verticesKey = line3DPainter->add_vertices(debug3DlinesPoints.size(), indices.size());
			auto vertices = line3DPainter->get_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.verticesKey);

			vertices->set_data<glm::vec3>(debug3DlinesPoints, std::string("position"));
			vertices->set_data<glm::vec3>(debug3DlinesColor, std::string("color"));
			vertices->set_indices(indices);

			debug3DlinesPoints.clear();
		}
		// 3D lines with depth
		if (debug3DlinesPointsDepth.size() != 0)
		{
			std::vector<unsigned int> indices;
			auto type = std::make_pair<GLenum, std::string>(GL_FLOAT_VEC3, "position");
			std::vector<std::pair < GLenum, std::string > > types;
			types.push_back(type);
			type = std::make_pair<GLenum, std::string>(GL_FLOAT_VEC3, "color");
			types.push_back(type);

			indices.resize(debug3DlinesPointsDepth.size());
			for (int i = 0; i < debug3DlinesPointsDepth.size(); ++i)
			{
				indices[i] = i;
			}
			if (!paintingManager->has_painter(types))
			{
				Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.painterKey = paintingManager->add_painter(std::move(types));
			}
			else
			{
				Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.painterKey = paintingManager->get_painter(types);
			}
			Key<Painter> kk = Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.painterKey;

			line3DPainterDepth = paintingManager->get_painter(Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.painterKey);

			Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.verticesKey = line3DPainterDepth->add_vertices(debug3DlinesPointsDepth.size(), indices.size());
			auto vertices = line3DPainterDepth->get_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.verticesKey);

			vertices->set_data<glm::vec3>(debug3DlinesPointsDepth, std::string("position"));
			vertices->set_data<glm::vec3>(debug3DlinesColorDepth, std::string("color"));
			vertices->set_indices(indices);

			debug3DlinesPointsDepth.clear();
		}
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

		auto type = std::make_pair<GLenum, std::string>(GL_FLOAT_VEC2, "position");
		std::vector<std::pair < GLenum, std::string > > types;
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

		vertices->set_data<glm::vec2>(positions, std::string("position"));
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

		auto type = std::make_pair<GLenum, std::string>(GL_FLOAT_VEC3, "position");
		std::vector<std::pair < GLenum, std::string > > types;
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

		vertices->set_data<glm::vec3>(positions, std::string("position"));
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

		registerCallback<Commands::ToRender::Draw2DLine>([&](Commands::ToRender::Draw2DLine& msg)
		{
			debug2DlinesPoints.push_back(msg.start);
			debug2DlinesPoints.push_back(msg.end);

			debug2DlinesColor.push_back(msg.color);
			debug2DlinesColor.push_back(msg.color);
		});

		registerCallback<Commands::ToRender::Draw2DQuad>([&](Commands::ToRender::Draw2DQuad& msg)
		{
			debug2DlinesPoints.push_back(msg.a);
			debug2DlinesPoints.push_back(msg.b);
			debug2DlinesPoints.push_back(msg.b);
			debug2DlinesPoints.push_back(msg.c);
			debug2DlinesPoints.push_back(msg.c);
			debug2DlinesPoints.push_back(msg.d);
			debug2DlinesPoints.push_back(msg.d);
			debug2DlinesPoints.push_back(msg.a);

			debug2DlinesColor.push_back(msg.color);
			debug2DlinesColor.push_back(msg.color);
			debug2DlinesColor.push_back(msg.color);
			debug2DlinesColor.push_back(msg.color);
			debug2DlinesColor.push_back(msg.color);
			debug2DlinesColor.push_back(msg.color);
			debug2DlinesColor.push_back(msg.color);
			debug2DlinesColor.push_back(msg.color);
		});

		registerCallback<Commands::ToRender::Draw3DLine>([&](Commands::ToRender::Draw3DLine& msg)
		{
			if (msg.depthTest)
			{
				debug3DlinesPointsDepth.push_back(msg.start);
				debug3DlinesPointsDepth.push_back(msg.end);

				debug3DlinesColorDepth.push_back(msg.color);
				debug3DlinesColorDepth.push_back(msg.endColor);
			}
			else
			{
				debug3DlinesPoints.push_back(msg.start);
				debug3DlinesPoints.push_back(msg.end);

				debug3DlinesColor.push_back(msg.color);
				debug3DlinesColor.push_back(msg.endColor);
			}
		});

		registerCallback<Commands::ToRender::Draw3DQuad>([&](Commands::ToRender::Draw3DQuad& msg)
		{
			if (msg.depthTest)
			{
				debug3DlinesPointsDepth.push_back(msg.a);
				debug3DlinesPointsDepth.push_back(msg.b);
				debug3DlinesPointsDepth.push_back(msg.b);
				debug3DlinesPointsDepth.push_back(msg.c);
				debug3DlinesPointsDepth.push_back(msg.c);
				debug3DlinesPointsDepth.push_back(msg.d);
				debug3DlinesPointsDepth.push_back(msg.d);
				debug3DlinesPointsDepth.push_back(msg.a);

				debug3DlinesColorDepth.push_back(msg.color);
				debug3DlinesColorDepth.push_back(msg.color);
				debug3DlinesColorDepth.push_back(msg.color);
				debug3DlinesColorDepth.push_back(msg.color);
				debug3DlinesColorDepth.push_back(msg.color);
				debug3DlinesColorDepth.push_back(msg.color);
				debug3DlinesColorDepth.push_back(msg.color);
				debug3DlinesColorDepth.push_back(msg.color);
			}
			else
			{
				debug3DlinesPoints.push_back(msg.a);
				debug3DlinesPoints.push_back(msg.b);
				debug3DlinesPoints.push_back(msg.b);
				debug3DlinesPoints.push_back(msg.c);
				debug3DlinesPoints.push_back(msg.c);
				debug3DlinesPoints.push_back(msg.d);
				debug3DlinesPoints.push_back(msg.d);
				debug3DlinesPoints.push_back(msg.a);

				debug3DlinesColor.push_back(msg.color);
				debug3DlinesColor.push_back(msg.color);
				debug3DlinesColor.push_back(msg.color);
				debug3DlinesColor.push_back(msg.color);
				debug3DlinesColor.push_back(msg.color);
				debug3DlinesColor.push_back(msg.color);
				debug3DlinesColor.push_back(msg.color);
				debug3DlinesColor.push_back(msg.color);
			}
		});

		registerCallback<AGE::DRBCameraDrawableListCommand>([&](AGE::DRBCameraDrawableListCommand &msg)
		{
			std::shared_ptr<Painter> line2DPainter = nullptr;
			std::shared_ptr<Painter> line3DPainter = nullptr;
			std::shared_ptr<Painter> line3DPainterDepth = nullptr;

			if (pipelines[msg.list->cameraInfos.data.pipeline]->isDebug())
			{
				fillDebugPainter(line2DPainter, line3DPainter, line3DPainterDepth);
			}
			pipelines[msg.list->cameraInfos.data.pipeline]->render(*msg.list.get());
			if (line2DPainter != nullptr)
			{
				line2DPainter->remove_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug2Dlines.verticesKey);
				debug2DlinesColor.clear();
				debug2DlinesPoints.clear();
			}
			if (line3DPainter != nullptr)
			{
				line3DPainter->remove_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug3Dlines.verticesKey);
				debug3DlinesColor.clear();
				debug3DlinesPoints.clear();
			}
			if (line3DPainterDepth != nullptr)
			{
				line3DPainterDepth->remove_vertices(Singleton<SimpleGeometryManager>::getInstance()->debug3DlinesDepth.verticesKey);
				debug3DlinesColorDepth.clear();
				debug3DlinesPointsDepth.clear();
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

		std::chrono::system_clock::time_point waitStart;
		std::chrono::system_clock::time_point waitEnd;
		std::chrono::system_clock::time_point workStart;
		std::chrono::system_clock::time_point workEnd;

		while (_run && _insideRun)
		{
			SCOPE_profile_cpu_i("RenderTimer", "Update");

			workStart = std::chrono::high_resolution_clock::now();

			SCOPE_profile_cpu_i("RenderTimer", "Get and execute tasks");
			waitStart = std::chrono::high_resolution_clock::now();
			TMQ::MessageBase *task = nullptr;
			
			while (TMQ::TaskManager::RenderThreadGetTask(task) == false)
			{ }

			waitEnd = std::chrono::high_resolution_clock::now();
			if (task)
			{
				SCOPE_profile_cpu_i("RenderTimer", "Execute task");
				auto success = execute(task); // we receive a task that we cannot treat
				AGE_ASSERT(success);
			}

			workEnd = std::chrono::high_resolution_clock::now();
			auto waitCount = std::chrono::duration_cast<std::chrono::microseconds>(waitEnd - waitStart).count();
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