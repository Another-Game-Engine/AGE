#include "PrepareRenderThread.hpp"
#include <Utils/ThreadName.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/AScene.hh>
#include <Threads/Tasks/MainToPrepareTasks.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>
#include <microprofile/microprofile.h>
#include <Utils/Profiler.hpp>

namespace AGE
{
	PrepareRenderThread::PrepareRenderThread()
		: Thread(AGE::Thread::ThreadType::PrepareRender)
		, _activeScene(nullptr)
	{}

	PrepareRenderThread::~PrepareRenderThread()
	{}

	bool PrepareRenderThread::init()
	{
		registerCallback<Tasks::MainToPrepare::CreateScene>([this](Tasks::MainToPrepare::CreateScene &msg){
		});

		registerCallback<Commands::MainToPrepare::SetCurrentScene>([this](Commands::MainToPrepare::SetCurrentScene &msg){
		});

		registerCallback<Commands::MainToPrepare::CameraInfos>([this](Commands::MainToPrepare::CameraInfos &msg){
		});

		registerCallback<Commands::MainToPrepare::CreateCamera>([this](Commands::MainToPrepare::CreateCamera &msg){
		});

		registerCallback<Commands::MainToPrepare::CreateMesh>([this](Commands::MainToPrepare::CreateMesh &msg){
		});

		registerCallback<Commands::MainToPrepare::CreatePointLight>([this](Commands::MainToPrepare::CreatePointLight &msg){
		});

		registerCallback<Commands::MainToPrepare::CreateSpotLight>([this](Commands::MainToPrepare::CreateSpotLight &msg){
		});

		registerCallback<Commands::MainToPrepare::CreateDirectionalLight>([this](Commands::MainToPrepare::CreateDirectionalLight &msg){
		});

		registerCallback<Commands::MainToPrepare::DeleteCamera>([this](Commands::MainToPrepare::DeleteCamera &msg){
		});

		registerCallback<Commands::MainToPrepare::DeleteMesh>([this](Commands::MainToPrepare::DeleteMesh &msg){
		});

		registerCallback<Commands::MainToPrepare::DeletePointLight>([this](Commands::MainToPrepare::DeletePointLight &msg){
		});

		registerCallback<Commands::MainToPrepare::DeleteSpotLight>([this](Commands::MainToPrepare::DeleteSpotLight &msg){
		});

		registerCallback<Commands::MainToPrepare::DeleteDirectionalLight>([this](Commands::MainToPrepare::DeleteDirectionalLight &msg){
		});

		registerCallback<Commands::MainToPrepare::PrepareDrawLists>([this](Commands::MainToPrepare::PrepareDrawLists &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_prepareDrawList(msg);
		});

		registerCallback<Commands::MainToPrepare::SetGeometry>([this](Commands::MainToPrepare::SetGeometry &msg){
		});

		registerCallback<Commands::MainToPrepare::SetRenderMode>([this](Commands::MainToPrepare::SetRenderMode &msg){
		});

		registerCallback<Commands::MainToPrepare::SetPointLight>([this](Commands::MainToPrepare::SetPointLight &msg){
		});

		registerCallback<Commands::MainToPrepare::SetDirectionalLight>([this](Commands::MainToPrepare::SetDirectionalLight &msg){
		});

		registerCallback<Commands::MainToPrepare::SetSpotLight>([this](Commands::MainToPrepare::SetSpotLight &msg){
		});

		registerCallback<Commands::MainToPrepare::SetTransform>([this](Commands::MainToPrepare::SetTransform &msg){
		});

		registerCallback<Tasks::Basic::VoidFunction>([this](Tasks::Basic::VoidFunction &msg){
			if (msg.function)
				msg.function();
		});

		registerCallback<Tasks::Basic::Exit>([&](Tasks::Basic::Exit& msg)
		{
			_insideRun = false;
			GetRenderThread()->stop();
		});

		return true;
	}

	bool PrepareRenderThread::release()
	{
		return true;
	}

	bool PrepareRenderThread::launch()
	{
		if (!init())
			return false;
		_threadHandle = std::thread(&PrepareRenderThread::update, std::ref(*this));
		MicroProfileOnThreadExit();
		return true;
	}

	bool PrepareRenderThread::stop()
	{
		getQueue()->emplaceTask<Tasks::Basic::Exit>();
		if (_threadHandle.joinable())
			_threadHandle.join();
		return true;
	}

	bool PrepareRenderThread::update()
	{
		/*
		- Tant qu'il n'y a pas de command
		    -> je pop des task
		- Une fois que j'ai mes commandes
            -> pour chacunes d'elles
			    -> je regarde si c'est a moi de les traiter (ex : changement de scene)
				-> si ca n'est pas a moi de les traiter
				    -> je les passe a la scene active
					-> si il n'y a pas de scene active, c'est que j'ai fait une erreur, et j'assert dans ta face :D
		- Si le render thread est pret, je lui envoi ses commandes
		- Sinon je ne lui envoie pas et je clear ma liste de commandes
		*/

		TMQ::PtrQueue commands;
		TMQ::PtrQueue tasks;
		bool commandSucces;
		bool taskSuccess;

		_registerId();

		_run = true;
		_insideRun = true;
		DWORD threadId = GetThreadId(static_cast<HANDLE>(_threadHandle.native_handle()));
		SetThreadName(threadId, this->_name.c_str());
		_next->getQueue()->reserveTo(std::this_thread::get_id().hash());

		while (this->_run && _insideRun)
		{
			SCOPE_profile_cpu_i("PrepareTimer", "Update");

			std::chrono::system_clock::time_point waitStart;
			std::chrono::system_clock::time_point waitEnd;
			std::chrono::system_clock::time_point workStart;
			std::chrono::system_clock::time_point workEnd;

			waitStart = std::chrono::high_resolution_clock::now();
			{
				SCOPE_profile_cpu_i("PrepareTimer", "Wait and get commands and tasks");
				getQueue()->getTaskAndCommandQueue(tasks, taskSuccess, commands, commandSucces, TMQ::HybridQueue::WaitType::Block);
			}
			waitEnd = std::chrono::high_resolution_clock::now();
			workStart = std::chrono::high_resolution_clock::now();

			if (taskSuccess)
			{
				SCOPE_profile_cpu_i("PrepareTimer", "Execute tasks");
				while (!tasks.empty())
				{
					auto task = tasks.front();
					auto success = execute(task); // we receive a task that we cannot treat
					AGE_ASSERT(success);
					tasks.pop();
					taskCounter--;
				}
			}
			if (commandSucces)
			{
				SCOPE_profile_cpu_i("PrepareTimer", "Execute commands");
				while (!commands.empty())
				{
					auto command = commands.front();
					if (!execute(command))
					{
						_next->getQueue()->moveCommand(command, commands.getFrontSize());
						commands.pop();
						continue;
					}
					commands.pop();
				}
				{
					SCOPE_profile_cpu_i("PrepareTimer", "Release commands for render thread");
					if (!_next->getQueue()->releaseCommandReadability(TMQ::HybridQueue::WaitType::NoWait))
					{
						_next->getQueue()->clear();
					}
				}
			}
			workEnd = std::chrono::high_resolution_clock::now();
			GetThreadManager()->updateThreadStatistics(this->_id
				, std::chrono::duration_cast<std::chrono::microseconds>(workEnd - workStart).count()
				, std::chrono::duration_cast<std::chrono::microseconds>(waitEnd - waitStart).count());
		}
		return true;
	}

	RenderScene *PrepareRenderThread::_getRenderScene(const AScene *scene)
	{
		for (auto &e : _scenes)
		{
			if (e->getScene() == scene)
			{
				return e.get();
			}
		}
		return nullptr;
	}

	void PrepareRenderThread::setCameraInfos(CameraData const &data, const PrepareKey &key)
	{
		getQueue()->emplaceCommand<Commands::MainToPrepare::CameraInfos>(key, data);
	}

	PrepareKey PrepareRenderThread::addCamera()
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		return scene->addCamera();
	}

	void PrepareRenderThread::updateGeometry(
		const PrepareKey &key
		, const Vector<SubMeshInstance> &meshs
		, const AGE::Vector<MaterialInstance> &materials)
	{
		assert(!key.invalid() || key.type != PrepareKey::Type::Mesh);
		getQueue()->emplaceCommand<Commands::MainToPrepare::SetGeometry>(key, meshs, materials);
	}

	void PrepareRenderThread::updateRenderMode(
		const PrepareKey &key
		, const RenderModeSet &renderModes)
	{
		AGE_ASSERT(!key.invalid() || key.type != PrepareKey::Type::Mesh);
		getQueue()->emplaceCommand<Commands::MainToPrepare::SetRenderMode>(key, renderModes);
	}

	PrepareKey PrepareRenderThread::addMesh()
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		auto key = scene->addMesh();
		getQueue()->emplaceCommand<Commands::MainToPrepare::CreateMesh>(key);
		return key;
	}

	PrepareKey PrepareRenderThread::addSpotLight()
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		auto key = scene->addSpotLight();
		getQueue()->emplaceCommand<Commands::MainToPrepare::CreateSpotLight>(key);
		return key;
	}

	PrepareKey PrepareRenderThread::addPointLight()
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		auto key = scene->addPointLight();
		getQueue()->emplaceCommand<Commands::MainToPrepare::CreatePointLight>(key);
		return key;
	}

	PrepareKey PrepareRenderThread::addDirectionalLight()
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		auto key = scene->addDirectionalLight();
		getQueue()->emplaceCommand<Commands::MainToPrepare::CreateDirectionalLight>(key);
		return key;
	}

	void PrepareRenderThread::setPointLight(PointLightData const &data, const PrepareKey &key)
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		getQueue()->emplaceCommand<Commands::MainToPrepare::SetPointLight>(data, key);
	}

	void PrepareRenderThread::setDirectionalLight(DirectionalLightData const &data, PrepareKey const &key)
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		getQueue()->emplaceCommand<Commands::MainToPrepare::SetDirectionalLight>(data, key);
	}

	void PrepareRenderThread::setSpotLight(SpotLightData const &data, const PrepareKey &key)
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		getQueue()->emplaceCommand<Commands::MainToPrepare::SetSpotLight>(data, key);
	}

	void PrepareRenderThread::_createRenderScene(AScene *scene)
	{
		_scenes.emplace_back(std::make_unique<RenderScene>(this, scene->getEngine(), scene));
		scene->setRenderScene(_scenes.back().get());
	}

	void PrepareRenderThread::_destroyRenderScene(AGE::RenderScene *scene)
	{
		assert(false);
	}

}