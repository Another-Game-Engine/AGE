#include "FileSystemThread.hpp"
#include "Engine.hpp"
#include "FileSystem.hpp"

namespace Age
{
	namespace Engine
	{
		void FileSystemThread::process(void)
		{
			while (isRunning())
			{
				Engine::GetInstance()->getFileSystem()->update();
			}
		}
	}
}
