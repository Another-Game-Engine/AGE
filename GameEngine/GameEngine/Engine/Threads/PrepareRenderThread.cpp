#include "PrepareRenderThread.hpp"
#include <Utils/ThreadName.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/AScene.hh>
#include <Core/Tasks/MainToPrepare.hpp>
#include <Core/Tasks/Basics.hpp>
#include <Threads/RenderThread.hpp>
#include <Core/Commands/Render.hpp>

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

		registerCallback<Commands::MainToPrepare::SceneUpdateBegin>([this](Commands::MainToPrepare::SceneUpdateBegin &msg){
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

		registerCallback<Commands::MainToPrepare::CreateDrawable>([this](Commands::MainToPrepare::CreateDrawable &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_createDrawable(msg);
		});

		registerCallback<Commands::MainToPrepare::CreatePointLight>([this](Commands::MainToPrepare::CreatePointLight &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_createPointLight(msg);
		});

		registerCallback<Commands::MainToPrepare::DeleteCamera>([this](Commands::MainToPrepare::DeleteCamera &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_deleteCamera(msg);
		});

		registerCallback<Commands::MainToPrepare::DeleteDrawable>([this](Commands::MainToPrepare::DeleteDrawable &msg){
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

		registerCallback<Commands::MainToPrepare::SetOrientation>([this](Commands::MainToPrepare::SetOrientation &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_setOrientation(msg);
		});

		registerCallback<Commands::MainToPrepare::SetPointLight>([this](Commands::MainToPrepare::SetPointLight &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_setPointLight(msg);
		});

		registerCallback<Commands::MainToPrepare::SetPosition>([this](Commands::MainToPrepare::SetPosition &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_setPosition(msg);
		});

		registerCallback<Commands::MainToPrepare::SetScale>([this](Commands::MainToPrepare::SetScale &msg){
			assert(this->_activeScene != nullptr);
			_activeScene->_setScale(msg);
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
					assert(execute(task)); // we receive a task that we cannot treat
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

	void PrepareRenderThread::setCameraProjection(const glm::mat4& projection, const PrepareKey &key)
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		scene->setCameraInfos(key, projection);
	}

	PrepareKey PrepareRenderThread::addCamera()
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		return scene->addCamera();
	}

	void PrepareRenderThread::updateGeometry(
		const PrepareKey &key
		, const AGE::Vector<AGE::SubMeshInstance> &meshs)
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		scene->updateGeometry(key, meshs);
	}

	PrepareKey PrepareRenderThread::addMesh()
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		return scene->addMesh();
	}

	PrepareKey PrepareRenderThread::addPointLight()
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		return scene->addPointLight();
	}

	void PrepareRenderThread::setPointLight(glm::vec3 const &color, glm::vec3 const &range, const PrepareKey &id)
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		scene->setPointLight(color, range, id);
	}

	void PrepareRenderThread::_createRenderScene(AScene *scene)
	{
		_scenes.emplace_back(std::make_unique<RenderScene>(this, scene->getEngine().lock().get(), scene));
		scene->setRenderScene(_scenes.back().get());
	}

	void PrepareRenderThread::_destroyRenderScene(AGE::RenderScene *scene)
	{
		assert(false);
	}

}