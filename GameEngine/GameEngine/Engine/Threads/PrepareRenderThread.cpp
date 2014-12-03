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

		bool commandsCleared = false;
		while (!commandsCleared)
		{
			if (!tasks.empty())
			{
				// pop one task and execute
			}
			else
			{
				getTaskQueue()->getReadableQueue(tasks);
			}
			if (commands.empty())
			{
				getCommandQueue()->getReadableQueue(commands);
			}
			else
			{
				// pop all commands
				commandsCleared = true;
				if (!_next->getCommandQueue()->releaseReadability(TMQ::ReleasableQueue::WaitType::NoWait))
				{
					_next->getCommandQueue()->clear();
				}
			}
		}

		while (!tasks.empty())
		{
			//pop all tasks
		}

		return true;
	}
}