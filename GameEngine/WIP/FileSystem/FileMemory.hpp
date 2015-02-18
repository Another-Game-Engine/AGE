#pragma once

#include "FileInterface.hpp"

namespace AGE
{
	namespace Engine
	{
		class FileMemory : public FileInterface
		{
		public:
			FileMemory(const char *name, unsigned char *data, std::size_t size, bool owner);
			virtual ~FileMemory(void);

			virtual bool endOfFile(void) const override final;
			virtual int getCharacter(void) override final;
			virtual void seekSet(int offset) override final;
			virtual void seekEnd(int offset) override final;
			virtual void seekCurrent(int offset) override final;
			virtual std::size_t tell(void) const override final;
			virtual bool flush(void) const override final;
			virtual std::size_t read(void *ptr, std::size_t size, std::size_t nmemb) override final;
			virtual std::size_t write(const void *ptr, std::size_t size, std::size_t nmemb) override final;
			virtual const char *getName(void) const override final;
			virtual std::size_t getSize(void) override final;

		private:
			const char *name;
			unsigned char *data = nullptr;
			unsigned char *ptr = nullptr;
			std::size_t size = 0;
			bool owner = false;
		};
	}
}