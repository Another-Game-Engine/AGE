#include "FileSystemThread.hpp"
#include "FileSystem.hpp"

#include <Utils/EngineBase.hpp>

namespace AGE
{
	void FileSystemThread::process(void)
	{
		while (isRunning())
		{
			g_engineInstance->getInstance<FileSystem>()->update();
		}
	}
}
