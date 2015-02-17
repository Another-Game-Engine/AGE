#pragma once

#include <cstddef>

#include "Platform.hpp"

namespace Age
{
	namespace Engine
	{
		class FileInterface
		{
		public:
			virtual ~FileInterface(void) = default;

			virtual bool endOfFile(void) const = 0;
			virtual int getCharacter(void) = 0;
			virtual void seekSet(int offset) = 0;
			virtual void seekEnd(int offset) = 0;
			virtual void seekCurrent(int offset) = 0;
			virtual std::size_t tell(void) const = 0;
			virtual bool flush(void) const = 0;
			virtual std::size_t read(void *ptr, std::size_t size, std::size_t nmemb) = 0;
			virtual std::size_t write(const void *ptr, std::size_t size, std::size_t nmemb) = 0;
			virtual const char *getName(void) const = 0;
			virtual std::size_t getSize(void) = 0;
		};
	}
}