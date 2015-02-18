#include "FileSystemThread.hpp"
#include "FileSystem.hpp"

#include <Core/Engine.hh>

namespace AGE
{
	void FileSystemThread::process(void)
	{
		while (isRunning())
		{
			GetEngine()->getInstance<FileSystem>()->update();
		}
	}
}
