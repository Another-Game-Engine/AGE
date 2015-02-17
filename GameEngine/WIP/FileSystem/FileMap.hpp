#pragma once

#include <memory>

#include "FileMemory.hpp"

namespace Age
{
	namespace Engine
	{
		class FileMap final : public FileMemory
		{
		public:
			static std::shared_ptr<FileInterface> Create(const char *name);

			virtual ~FileMap(void);

		private:
			static const std::size_t BufferSize = 1024;

			unsigned char *data = nullptr;
			std::size_t size = 0;
			HANDLE file;
			HANDLE mapping;

			FileMap(const char *name, unsigned char *data, std::size_t size, HANDLE file, HANDLE mapping);
		};
	}
}