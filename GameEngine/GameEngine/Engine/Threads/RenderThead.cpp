#include "RenderThread.hpp"

namespace AGE
{
	RenderThread::RenderThread()
		: Thread(AGE::Thread::ThreadType::Render)
		, _activeContext(nullptr)
	{}

	RenderThread::~RenderThread()
	{}

	bool RenderThread::init()
	{
		return true;
	}

	bool RenderThread::release()
	{
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
			}
		}

		while (!tasks.empty())
		{
			//pop all tasks
		}

		return true;
	}
}