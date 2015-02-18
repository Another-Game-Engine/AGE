#include "RenderThread.hpp"
#include <Core/Engine.hh>
#include <Context/SdlContext.hh>
#include <Utils/ThreadName.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Context/SdlContext.hh>
#include <Utils/Containers/Vector.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/Engine.hh>
#include <Context/SdlContext.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Pipelining/Pipelines/CustomPipeline/BasicPipeline.hh>
#include <Utils/OpenGL.hh>
#include <Utils/Age_Imgui.hpp>
#include <Render/Properties/Transformation.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>

namespace AGE
{
	RenderThread::RenderThread()
		: Thread(AGE::Thread::ThreadType::Render)
		, _context(nullptr),
		paintingManager(std::make_shared<PaintingManager>()),
		pipelines(1)
	{
	}

	RenderThread::~RenderThread()
	{}

	bool RenderThread::init()
	{
		registerCallback<Tasks::Render::CreateRenderContext>([this](Tasks::Render::CreateRenderContext &msg)
		{
			_context = msg.engine->setInstance<SdlContext, IRenderContext>();
			if (!_context->init(0, 1920, 1000, "~AGE~ V0.00001 Demo"))
			{
				msg.setValue(false);
				return;
			}
			pipelines[0] = std::make_unique<BasicPipeline>(paintingManager);
			msg.setValue(true);
		});

 		registerCallback<Commands::ToRender::Flush>([&](Commands::ToRender::Flush& msg)
		{
			_context->swapContext();
			glClear(GL_COLOR_BUFFER_BIT);
		});

		registerCallback<Tasks::Render::SetMeshTransform>([&](Tasks::Render::SetMeshTransform &msg)
		{
			auto painter = paintingManager->get_painter(msg.painter);
			auto vertices = painter->get_vertices(msg.mesh);
			auto property = vertices->get_property<Transformation>(msg.transform);
			
			assert(property != nullptr);
			*property = msg.transformMat;
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
			_drawlist = msg.list;
		});

		registerCallback<Commands::ToRender::RenderDrawLists>([&](Commands::ToRender::RenderDrawLists& msg)
		{
			uint32_t pipelineIdx = 0;

			for (auto &curPipeline : pipelines) {
				for (auto &curCamera : _drawlist) {
					if (pipelineIdx < curCamera.pipelines.size()) {
						curPipeline->render(curCamera.pipelines[pipelineIdx], curCamera.lights, curCamera.camInfos);
					}
				}
				++pipelineIdx;
			}
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

		registerCallback<Tasks::Render::SetMeshMaterial>([&](Tasks::Render::SetMeshMaterial& msg)
		{
			for (auto &subMesh : msg.mesh->subMeshs)
			{
				auto vertices = paintingManager->get_painter(subMesh.painter)->get_vertices(subMesh.vertices);
				assert(vertices != nullptr);
				if (subMesh.defaultMaterialIndex >= msg.material->datas.size())
				{
					for (auto &prop : msg.material->datas[0])
					{
						prop->set_program(pipelines[0]->get_programs());
						vertices->add_property(prop);
					}
				}
				else
				{
					for (auto &prop : msg.material->datas[subMesh.defaultMaterialIndex])
					{
						prop->set_program(pipelines[0]->get_programs());
						vertices->add_property(prop);
					}
				}
			}
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
			if (_context)
				_context->refreshInputs();
			waitStart = std::chrono::high_resolution_clock::now();
			getQueue()->getTaskAndCommandQueue(tasks, taskSuccess, commands, commandSuccess, TMQ::HybridQueue::Block);
			waitEnd = std::chrono::high_resolution_clock::now();
			workStart = std::chrono::high_resolution_clock::now();
			if (taskSuccess)
			{
				while (!tasks.empty() && _insideRun)
				{
					//pop all tasks
					auto task = tasks.front();
					assert(execute(task)); // we receive a task that we cannot treat
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
					assert(execute(command));
					commands.pop();
				}
				_drawlist.clear();
			}
			workEnd = std::chrono::high_resolution_clock::now();
			GetThreadManager()->updateThreadStatistics(this->_id
				, std::chrono::duration_cast<std::chrono::microseconds>(workEnd - workStart).count()
				, std::chrono::duration_cast<std::chrono::microseconds>(waitEnd - waitStart).count());
		}
		return true;
	}
}