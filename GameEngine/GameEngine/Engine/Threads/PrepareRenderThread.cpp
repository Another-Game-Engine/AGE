#include "PrepareRenderThread.hpp"
#include <Utils/ThreadName.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/AScene.hh>
#include <Threads/Tasks/MainToPrepareTasks.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>

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
			this->_createRenderScene(msg.scene);
			msg.setValue(true);
		});

		registerCallback<Commands::MainToPrepare::SetCurrentScene>([this](Commands::MainToPrepare::SetCurrentScene &msg){
			this->_activeScene = _getRenderScene(msg.scene);
			assert(this->_activeScene != nullptr);
		});

		registerCallback<Commands::MainToPrepare::CameraInfos>([this](Commands::MainToPrepare::CameraInfos &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_setCameraInfos(msg);
		});

		registerCallback<Commands::MainToPrepare::CreateCamera>([this](Commands::MainToPrepare::CreateCamera &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_createCamera(msg);
		});

		registerCallback<Commands::MainToPrepare::CreateMesh>([this](Commands::MainToPrepare::CreateMesh &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_createMesh(msg);
		});

		registerCallback<Commands::MainToPrepare::CreatePointLight>([this](Commands::MainToPrepare::CreatePointLight &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_createPointLight(msg);
		});

		registerCallback<Commands::MainToPrepare::DeleteCamera>([this](Commands::MainToPrepare::DeleteCamera &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_deleteCamera(msg);
		});

		registerCallback<Commands::MainToPrepare::DeleteMesh>([this](Commands::MainToPrepare::DeleteMesh &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_deleteDrawable(msg);
		});

		registerCallback<Commands::MainToPrepare::DeletePointLight>([this](Commands::MainToPrepare::DeletePointLight &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_deletePointLight(msg);
		});

		registerCallback<Commands::MainToPrepare::PrepareDrawLists>([this](Commands::MainToPrepare::PrepareDrawLists &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_prepareDrawList(msg);
		});

		registerCallback<Commands::MainToPrepare::SetGeometry>([this](Commands::MainToPrepare::SetGeometry &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_setGeometry(msg);
		});

		registerCallback<Commands::MainToPrepare::SetPointLight>([this](Commands::MainToPrepare::SetPointLight &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_setPointLight(msg);
		});

		registerCallback<Commands::MainToPrepare::SetTransform>([this](Commands::MainToPrepare::SetTransform &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_setTransform(msg);
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
			std::chrono::system_clock::time_point waitStart;
			std::chrono::system_clock::time_point waitEnd;
			std::chrono::system_clock::time_point workStart;
			std::chrono::system_clock::time_point workEnd;

			waitStart = std::chrono::high_resolution_clock::now();
			getQueue()->getTaskAndCommandQueue(tasks, taskSuccess, commands, commandSucces, TMQ::HybridQueue::WaitType::Block);
			waitEnd = std::chrono::high_resolution_clock::now();
			workStart = std::chrono::high_resolution_clock::now();
			if (taskSuccess)
			{
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
				if (!_next->getQueue()->releaseCommandReadability(TMQ::HybridQueue::WaitType::NoWait))
				{
					_next->getQueue()->clear();
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

	void PrepareRenderThread::setCameraInfos(const glm::mat4& projection, const PrepareKey &key, RenderType pipeline)
	{
		getQueue()->emplaceCommand<Commands::MainToPrepare::CameraInfos>(key, projection, pipeline);
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

	void PrepareRenderThread::setPointLight(glm::vec3 const &color, glm::vec3 const &range, std::shared_ptr<ITexture> const &texture, const PrepareKey &key)
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		getQueue()->emplaceCommand<Commands::MainToPrepare::SetPointLight>(color, range, texture, key);
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