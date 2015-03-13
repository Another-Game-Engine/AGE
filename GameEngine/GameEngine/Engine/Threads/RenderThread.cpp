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
#include <Render/Pipelining/Pipelines/CustomPipeline/BasicPipeline.hh>
#include <Render/Pipelining/Pipelines/CustomPipeline/DeferredShading.hh>
#include <Utils/OpenGL.hh>
#include <Utils/Age_Imgui.hpp>
#include <Render/Properties/Transformation.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <Utils/Debug.hpp>
#include <Render/GeometryManagement/SimpleGeometry.hpp>

namespace AGE
{
	RenderThread::RenderThread()
		: Thread(AGE::Thread::ThreadType::Render)
		, _context(nullptr),
		paintingManager(std::make_shared<PaintingManager>()),
		pipelines(2)
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

	void RenderThread::getQuadGeometry(Key<Vertices> &v, Key<Painter> &p)
	{
		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

		if (Quad::VerticesKey.isValid() && Quad::PainterKey.isValid())
		{
			v = Quad::VerticesKey;
			p = Quad::PainterKey;
			return;
		}

		auto type = std::make_pair<GLenum, std::string>(GL_FLOAT_VEC4, "position");
		std::vector<std::pair < GLenum, std::string > > types;
		types.push_back(type);

		if (!paintingManager->has_painter(types))
		{
			Quad::PainterKey = paintingManager->add_painter(std::move(types));
		}
		else
		{
			Quad::PainterKey = paintingManager->get_painter(types);
		}
		auto &painterPtr = paintingManager->get_painter(Quad::PainterKey);

		Quad::VerticesKey = painterPtr->add_vertices(Quad::Positions.size(), Quad::Indices.size());
		auto vertices = painterPtr->get_vertices(Quad::VerticesKey);

		vertices->set_data<glm::vec4>(Quad::Positions, std::string("position"));
		vertices->set_indices(Quad::Indices);

		v = Quad::VerticesKey;
		p = Quad::PainterKey;
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
			pipelines[DEFERRED] = std::make_unique<DeferredShading>(_context->getScreenSize(), paintingManager);
			pipelines[BASIC] = std::make_unique<BasicPipeline>(paintingManager);
			_recompileShaders();
			msg.setValue(true);
		});

 		registerCallback<Commands::ToRender::Flush>([&](Commands::ToRender::Flush& msg)
		{
			_context->swapContext();
			glClear(GL_COLOR_BUFFER_BIT);
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

		registerCallback<Commands::ToRender::CopyDrawLists>([&](Commands::ToRender::CopyDrawLists& msg)
		{
			_drawlistPtr = msg.listContainer;
		});

		registerCallback<Commands::ToRender::RenderDrawLists>([&](Commands::ToRender::RenderDrawLists& msg)
		{
			uint32_t pipelineIdx = 0;

			if (!_drawlistPtr) // nothing to draw
				return;
			AGE_ASSERT(_drawlistPtr != nullptr);

			for (auto &curPipeline : pipelines)
			{
				auto &drawlist = _drawlistPtr->container.cameras;
				for (auto &curCamera : drawlist)
				{
					if (pipelineIdx < curCamera.pipelines.size()) {
						curPipeline->render(curCamera.pipelines[pipelineIdx], curCamera.lights, curCamera.camInfos);
					}
				}
				++pipelineIdx;
			}
			_drawlistPtr = nullptr;
		});

		registerSharedCallback<AGE::Tasks::Basic::BoolFunction>([&](AGE::Tasks::Basic::BoolFunction& msg)
		{
			msg.setValue(msg.function());
		});

		registerCallback<AGE::Tasks::Basic::VoidFunction>([&](AGE::Tasks::Basic::VoidFunction& msg)
		{
			if (msg.function)
				msg.function();
		});

		registerCallback<AGE::Tasks::Basic::Exit>([&](AGE::Tasks::Basic::Exit& msg)
		{
			AGE::CreateEngine()->deleteInstance<IRenderContext>();
			this->_insideRun = false;
		});

#ifdef USE_IMGUI
		registerCallback<AGE::RenderImgui>([&](AGE::RenderImgui &msg)
		{
			AGE::Imgui::getInstance()->renderThreadRenderFn(msg.cmd_lists);
		});
#endif

		registerCallback<AGE::Tasks::Render::ContextGrabMouse>([&](AGE::Tasks::Render::ContextGrabMouse &msg)
		{
			_context->grabMouse(msg.grabMouse == 1 ? true : false);
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
			waitStart = std::chrono::high_resolution_clock::now();
			taskSuccess = commandSuccess = false;
			do {
				if (_context)
					_context->refreshInputs();
				getQueue()->getTaskAndCommandQueue(tasks, taskSuccess, commands, commandSuccess, TMQ::HybridQueue::Block);
			} while (!taskSuccess && !commandSuccess);
			waitEnd = std::chrono::high_resolution_clock::now();
			workStart = std::chrono::high_resolution_clock::now();
			if (taskSuccess)
			{
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
				// pop all commands
				while (!commands.empty() && _insideRun)
				{
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