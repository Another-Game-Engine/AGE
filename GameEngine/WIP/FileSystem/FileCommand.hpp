#pragma once

#include <memory>
#include <string>

#include "FileInterface.hpp"

namespace Age
{
	namespace Engine
	{
		class FileCommand final : public FileInterface
		{
		public:
			static std::shared_ptr<FileInterface> Create(const char *command, const char *mode);

			virtual ~FileCommand(void);

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
			FileCommand(const char *name, FILE *file);

			std::string command;
			FILE *file = nullptr;
		};
	}
}