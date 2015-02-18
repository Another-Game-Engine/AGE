#pragma once

#include "Thread.hpp"

namespace AGE
{
	namespace Engine
	{
		class FileSystemThread : public Thread
		{
		public:
			FileSystemThread(void) = default;
			FileSystemThread(const FileSystemThread &other) = delete;
			FileSystemThread(FileSystemThread &&other) = delete;
			FileSystemThread &operator=(const FileSystemThread &other) = delete;
			FileSystemThread &operator=(FileSystemThread &&other) = delete;
			virtual ~FileSystemThread(void) = default;

		protected:
			virtual void process(void) override final;
		};
	}
}