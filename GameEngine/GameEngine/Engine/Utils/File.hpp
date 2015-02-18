#pragma once

#include <memory>

#include "Stream.hpp"

namespace AGE
{
	namespace Engine
	{
		class File final : public Stream
		{
		public:
			File(void);
			File(const File &other);
			File(File &&other);
			File &operator=(const File &other);
			File &operator=(File &&other);
			virtual ~File(void);

			File &swap(File &other);

			bool open(const char *name, const char *mode);
			bool execute(const char *command, const char *mode);
			void close(void);

			const char *getName(void) const;
			std::size_t getSize(void) const;

			virtual bool isOpened(void) const override final;
			virtual std::size_t read(void *ptr, std::size_t size, std::size_t nmemb) const override final;
			virtual std::size_t write(const void *ptr, std::size_t size, std::size_t nmemb) const override final;

			virtual bool endOfFile(void) const override final;
			virtual int getCharacter(void) const override final;
			virtual void seekSet(int offset) const override final;
			virtual void seekEnd(int offset) const override final;
			virtual void seekCurrent(int offset) const override final;
			virtual std::size_t tell(void) const override final;
			virtual bool flush(void) const override final;

			using Stream::read;
			using Stream::readLittle;
			using Stream::readBig;
			using Stream::write;
			using Stream::writeLittle;
			using Stream::writeBig;

		private:
			struct FileData;
			std::shared_ptr<FileData> data;
		};

		void swap(File &lhs, File &rhs);
	}
}