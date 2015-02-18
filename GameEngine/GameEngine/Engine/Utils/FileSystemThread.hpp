#pragma once

#include "FSThreadBase.hpp"

namespace AGE
{
	class FileSystemThread : public FSThreadBase
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