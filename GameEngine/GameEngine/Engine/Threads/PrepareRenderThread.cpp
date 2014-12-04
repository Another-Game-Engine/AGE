#include "PrepareRenderThread.hpp"

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
		_threadHandle = std::thread([&](){
			this->_run = true;
			while (this->_run)
			{
				update();
			}
		});
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

		_registerId();

		if (!getCommandQueue()->getReadableQueue(commands))
		{
			getTaskQueue()->getReadableQueue(tasks);
			while (!tasks.empty())
			{
				auto task = tasks.front();
				assert(executeTask(task)); // we receive a task that we cannot treat
				tasks.pop();
			}
		}
		else
		{
			// pop all commands
			while (!commands.empty())
			{
				auto command = commands.front();
				if (!executeCommand(command))
				{
					if (!_activeScene /*|| !_activeScene->executeCommand(command)*/) // TO UNCOMMENT
					{
						_next->getCommandQueue()->move(command, commands.getFrontSize());
						commands.pop();
						continue;
					}
					commands.pop();
					continue;
				}
				commands.pop();
			}
			if (!_next->getCommandQueue()->releaseReadability(TMQ::ReleasableQueue::WaitType::NoWait))
			{
				_next->getCommandQueue()->clear();
			}
		}

		return true;
	}
}