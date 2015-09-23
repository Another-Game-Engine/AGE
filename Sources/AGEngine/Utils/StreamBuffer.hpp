#pragma once

#include <memory>

#include "Stream.hpp"

namespace AGE
{
	class StreamBuffer final : public Stream
	{
	public:
		StreamBuffer(void);
		StreamBuffer(const StreamBuffer &other);
		StreamBuffer(StreamBuffer &&other);
		StreamBuffer &operator=(const StreamBuffer &other);
		StreamBuffer &operator=(StreamBuffer &&other);
		virtual ~StreamBuffer(void);

		virtual bool isOpened(void) const override final;
		virtual std::size_t read(void *ptr, std::size_t size, std::size_t nmemb) const override final;
		virtual std::size_t write(const void *ptr, std::size_t size, std::size_t nmemb) const override final;

		void swap(StreamBuffer &other);

		void resize(std::size_t size) const;
		void allocate(std::size_t size) const;
		void reserve(std::size_t size) const;
		void clear(void) const;
		std::size_t size(void) const;
		unsigned char *getData(void) const;
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
		struct BufferData;
		std::shared_ptr<BufferData> data;
	};
}