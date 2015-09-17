#include <cassert>
#include <cstring>
#include <cstdio>

#include "FileMemory.hpp"

namespace AGE
{
	FileMemory::FileMemory(const char *name, unsigned char *data, std::size_t size, bool owner)
		: name(name), data(data), ptr(data), size(size), owner(owner)
	{
		return;
	}

	FileMemory::~FileMemory(void)
	{
		if (owner)
		{
			delete[] data;
		}
	}

	const char *FileMemory::getName(void) const
	{
		return name;
	}

	std::size_t FileMemory::getSize(void)
	{
		return size;
	}

	std::size_t FileMemory::read(void *p, std::size_t s, std::size_t nmemb)
	{
		const std::size_t bytes = s * nmemb;
		const std::size_t bytesToEnd = size - (ptr - data);
		if (bytes <= bytesToEnd)
		{
			std::memcpy(p, ptr, bytes);
			ptr += bytes;
			return nmemb;
		}
		else
		{
			std::memcpy(p, ptr, bytesToEnd);
			ptr += bytesToEnd;
			return bytesToEnd / s;
		}
	}

	std::size_t FileMemory::write(const void *p, std::size_t s, std::size_t nmemb)
	{
		assert(false && "Impossible to write into the memory file");
		return 0;
	}

	bool FileMemory::endOfFile(void) const
	{
		return ptr - data >= static_cast<int>(size);
	}

	int FileMemory::getCharacter(void)
	{
		if (ptr - data >= static_cast<int>(size))
		{
			return EOF;
		}
		else
		{
			return *ptr++;
		}
	}

	void FileMemory::seekSet(int offset)
	{
		ptr = data + offset;
		if (ptr < data)
		{
			ptr = data;
		}
		else if (ptr > data + size)
		{
			ptr = data + size;
		}
	}

	void FileMemory::seekEnd(int offset)
	{
		ptr = data + size - offset;
		if (ptr < data)
		{
			ptr = data;
		}
		else if (ptr > data + size)
		{
			ptr = data + size;
		}
	}

	void FileMemory::seekCurrent(int offset)
	{
		ptr += offset;
		if (ptr < data)
		{
			ptr = data;
		}
		else if (ptr > data + size)
		{
			ptr = data + size;
		}
	}

	std::size_t FileMemory::tell(void) const
	{
		return ptr - data;
	}

	bool FileMemory::flush(void) const
	{
		return true;
	}
}
