#include "PrepareRenderThread.hpp"
#include <Utils/ThreadName.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/AScene.hh>

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
		_run = false;
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

		this->_run = true;
		DWORD threadId = GetThreadId(static_cast<HANDLE>(_threadHandle.native_handle()));
		SetThreadName(threadId, this->_name.c_str());

		while (this->_run)
		{
			getQueue()->getTaskAndCommandQueue(tasks, taskSuccess, commands, commandSucces, TMQ::HybridQueue::WaitType::Block);
			if (taskSuccess)
			{
				while (!tasks.empty())
				{
					auto task = tasks.front();
					assert(execute(task)); // we receive a task that we cannot treat
					tasks.pop();
				}
			}
			if (commandSucces)
			{
				while (!commands.empty())
				{
					auto command = commands.front();
					if (!execute(command))
					{
						if (!_activeScene /*|| !_activeScene->executeCommand(command)*/) // TO UNCOMMENT
						{
							_next->getQueue()->moveCommand(command, commands.getFrontSize());
							commands.pop();
							continue;
						}
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
		, const AGE::Vector<AGE::SubMeshInstance> &meshs
		, const AGE::Vector<AGE::MaterialInstance> &materials
		, const gl::Key<AGE::AnimationInstance> &animation)
	{
		auto scene = _getRenderScene(GetMainThread()->getActiveScene());
		assert(scene != nullptr);
		scene->updateGeometry(key, meshs, materials, animation);
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
}