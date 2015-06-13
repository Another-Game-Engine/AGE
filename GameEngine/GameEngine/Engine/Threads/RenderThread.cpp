#include "RenderThread.hpp"
#include <Core/Engine.hh>
#include <Context/SDL/SdlContext.hh>
#include <Utils/ThreadName.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Utils/Containers/Vector.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/Engine.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Pipelining/Pipelines/CustomPipeline/DebugDeferredShading.hh>
#include <Render/Pipelining/Pipelines/CustomPipeline/DeferredShading.hh>
#include <Utils/OpenGL.hh>
#include <Utils/Age_Imgui.hpp>
#include <Render/Properties/Transformation.hh>
#include <Culling/Ouptut/RenderCamera.hh>
#include <Culling/Ouptut/RenderLight.hh>
#include <Culling/Ouptut/RenderPipeline.hh>
#include <Utils/Debug.hpp>
#include <Render/GeometryManagement/SimpleGeometry.hh>
#include <Utils/Profiler.hpp>
#include <utility>

//BFC
#include "Graphic/DRBCameraDrawableList.hpp"

namespace AGE
{
	RenderThread::RenderThread()
		: Thread(AGE::Thread::ThreadType::Render)
		, _context(nullptr),
		paintingManager(std::make_shared<PaintingManager>()),
		pipelines(RenderType::TOTAL)
	{
	}

	RenderThread::~RenderThread()
	{}

	void RenderThread::_recompileShaders()
	{
		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

		for (auto &e : pipelines)
		{
			if (!e)
			{
				continue;
			}
			e->recompileShaders();
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
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

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
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

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
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

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
			if (!_context->init(1280, 720, "~AGE~ V0.00001 Demo"))
			{
				msg.setValue(false);
				return;
			}

#ifdef DEBUG
			_depthMapManager.init(1280, 720, 4);
#else
			_depthMapManager.init(1280, 720, 2);
#endif

			msg.setValue(true);
		});

		registerCallback<Tasks::Render::InitRenderPipelines>([this](Tasks::Render::InitRenderPipelines &msg)
		{
			_context = msg.engine->setInstance<SdlContext, IRenderContext>();
			pipelines[DEFERRED] = std::make_unique<DeferredShading>(_context->getScreenSize(), paintingManager);
			pipelines[DEBUG_DEFERRED] = std::make_unique<DebugDeferredShading>(_context->getScreenSize(), paintingManager);
			_recompileShaders();
			msg.setValue(true);
		});

		registerCallback<Commands::ToRender::Flush>([&](Commands::ToRender::Flush& msg)
		{
			{
				SCOPE_profile_cpu_i("RenderTimer", "SwapContext");
				_context->swapContext();
				{
					SCOPE_profile_gpu_i("Clear buffer");
					glClear(GL_COLOR_BUFFER_BIT);
				}
			}
			MicroProfileFlip();
		});

		registerCallback<Tasks::Render::ReloadShaders>([&](Tasks::Render::ReloadShaders& msg)
		{
#ifdef AGE_DEBUG
			_recompileShaders();
#else
			std::cerr << "Error : You cannot recompile shader at runtime. This feature is enabled only in debug mode\n";
#endif
		});

		registerCallback<Tasks::Render::GetWindowSize>([&](Tasks::Render::GetWindowSize &msg)
		{
			msg.setValue(_context->getScreenSize());
		});

		registerCallback<Tasks::Render::SetWindowSize>([&](Tasks::Render::SetWindowSize& msg)
		{
			_context->setScreenSize(msg.size);
		});

		//registerCallback<Commands::ToRender::RenderDrawLists>([&](Commands::ToRender::RenderDrawLists& msg)
		//{
		//	if (_drawlists.empty()) // nothing to draw
		//		return;

		//	SCOPE_profile_cpu_i("RenderTimer", "RenderDrawLists");

		//	for (auto &_drawlistPtr : _drawlists)
		//	{
		//		SCOPE_profile_cpu_i("RenderTimer", "Render one DrawList");

		//		// DEBUG DRAW DEGUEULASSE
		//		std::shared_ptr<Painter> painterPtr = nullptr;
		//		{

		//			SCOPE_profile_cpu_i("RenderTimer", "RenderDebugLines");
		//			std::vector<unsigned int> indices;
		//			auto type = std::make_pair<GLenum, std::string>(GL_FLOAT_VEC2, "position");
		//			std::vector<std::pair < GLenum, std::string > > types;
		//			types.push_back(type);

		//			indices.resize(debug2DlinesPoints.size());
		//			for (int i = 0; i < debug2DlinesPoints.size(); ++i)
		//			{
		//				indices[i] = i;
		//			}
		//			if (!paintingManager->has_painter(types))
		//			{
		//				debug2Dlines.painterKey = paintingManager->add_painter(std::move(types));
		//			}
		//			else
		//			{
		//				debug2Dlines.painterKey = paintingManager->get_painter(types);
		//			}
		//			Key<Painter> kk = debug2Dlines.painterKey;

		//			painterPtr = paintingManager->get_painter(debug2Dlines.painterKey);

		//			debug2Dlines.verticesKey = painterPtr->add_vertices(debug2DlinesPoints.size(), indices.size());
		//			auto vertices = painterPtr->get_vertices(debug2Dlines.verticesKey);

		//			vertices->set_data<glm::vec2>(debug2DlinesPoints, std::string("position"));
		//			vertices->set_indices(indices);

		//			debug2DlinesPoints.clear();
		//		}

		//		// CEST VRAIMENT DEGUEULASSE...

		//		{
		//			SCOPE_profile_cpu_i("RenderTimer", "Render cameras");

		//			auto &drawlist = _drawlistPtr->container.cameras;
		//			for (auto &curCamera : drawlist)
		//			{
		//				SCOPE_profile_gpu_i("RenderCameraDrawList");
		//				SCOPE_profile_cpu_i("RenderTimer", "RenderCamera");
		//				AGE_ASSERT(!(pipelines[curCamera.camInfos.data.pipeline] == nullptr));
		//				//pipelines[curCamera.camInfos.data.pipeline]->render(curCamera.pipeline, curCamera.lights, curCamera.camInfos);
		//			}
		//		}

		//		painterPtr->remove_vertices(debug2Dlines.verticesKey);
		//	}
		//	_drawlists.clear();
		//});

		registerCallback<AGE::DRBCameraDrawableListCommand>([&](AGE::DRBCameraDrawableListCommand &msg){
			pipelines[RenderType::DEFERRED]->render(*msg.list.get());
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
			AGE::CreateEngine()->deleteInstance<IRenderContext>();
			this->_insideRun = false;
		});

#ifdef AGE_ENABLE_IMGUI
		registerCallback<AGE::RenderImgui>([&](AGE::RenderImgui &msg)
		{
			AGE::Imgui::getInstance()->renderThreadRenderFn(msg.cmd_lists);
		});
#endif

		registerCallback<AGE::Tasks::Render::ContextGrabMouse>([&](AGE::Tasks::Render::ContextGrabMouse &msg)
		{
			_context->grabMouse(msg.grabMouse == 1 ? true : false);
		});

		registerCallback<Commands::ToRender::Draw2DLine>([&](Commands::ToRender::Draw2DLine& msg)
		{
			debug2DlinesPoints.push_back(msg.start);
			debug2DlinesPoints.push_back(msg.end);
		});

		return true;
	}

	bool RenderThread::release()
	{
		return true;
	}

	bool RenderThread::launch()
	{
		if (!init())
			return false;
		_threadHandle = std::thread(&RenderThread::update, std::ref(*this));
		MicroProfileOnThreadExit();
		return true;
	}

	bool RenderThread::stop()
	{
		getQueue()->emplaceTask<Tasks::Basic::Exit>();
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

		TMQ::PtrQueue commands;
		TMQ::PtrQueue tasks;
		bool commandSuccess;
		bool taskSuccess;

		std::chrono::system_clock::time_point waitStart;
		std::chrono::system_clock::time_point waitEnd;
		std::chrono::system_clock::time_point workStart;
		std::chrono::system_clock::time_point workEnd;

		while (_run && _insideRun)
		{
			SCOPE_profile_cpu_i("RenderTimer", "Update");

			waitStart = std::chrono::high_resolution_clock::now();
			taskSuccess = commandSuccess = false;
			{
				SCOPE_profile_cpu_i("RenderTimer", "Wait and get commands");
				do {
					if (_context)
					{
						_context->refreshInputs();
					}
					getQueue()->getTaskAndCommandQueue(tasks, taskSuccess, commands, commandSuccess, TMQ::HybridQueue::Block);
				} while (!taskSuccess && !commandSuccess);
			}
			waitEnd = std::chrono::high_resolution_clock::now();
			workStart = std::chrono::high_resolution_clock::now();
			if (taskSuccess)
			{
				SCOPE_profile_cpu_i("RenderTimer", "Execute tasks");
				while (!tasks.empty() && _insideRun)
				{
					//pop all tasks
					auto task = tasks.front();
					auto success = execute(task); // we receive a task that we cannot treat
					AGE_ASSERT(success);
					tasks.pop();
					taskCounter--;
					workEnd = std::chrono::high_resolution_clock::now();
					const std::size_t toWait = 33;
					const std::size_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(workEnd - workStart).count();
					if (elapsed >= toWait)
					{
						std::cout << elapsed << ", ";
						while (!tasks.empty() && _insideRun)
						{
							auto task = tasks.front();
							getQueue()->moveTask(task, tasks.getFrontSize());
							tasks.pop();
						}
					}
				}
			}
			if (commandSuccess)
			{
				SCOPE_profile_cpu_i("RenderTimer", "Execute commands");
				// pop all commands
				while (!commands.empty() && _insideRun)
				{
					SCOPE_profile_cpu_i("RenderTimer", "Execute one command");
					auto command = commands.front();
					auto success = execute(command);
					AGE_ASSERT(success);
					commands.pop();
				}

				workEnd = std::chrono::high_resolution_clock::now();
				GetThreadManager()->updateThreadStatistics(this->_id
					, std::chrono::duration_cast<std::chrono::microseconds>(workEnd - workStart).count()
					, std::chrono::duration_cast<std::chrono::microseconds>(waitEnd - waitStart).count());
			}
		}
		return true;
	}
}