#include "FileMap.hpp"
#include "Encoding.hpp"

namespace AGE
{
	FileMap::FileMap(const char *name, unsigned char *data, std::size_t size, HANDLE file, HANDLE mapping)
		: FileMemory(name, data, size, 0), file(file), mapping(mapping), data(data), size(size)
	{
		return;
	}

	FileMap::~FileMap(void)
	{
		UnmapViewOfFile(data);
		CloseHandle(mapping);
		CloseHandle(file);
	}

	std::shared_ptr<FileInterface> FileMap::Create(const char *name)
	{
		std::shared_ptr<FileInterface> returnValue;
		wchar_t nameBuf[BufferSize];
		Encoding::Utf8ToUnicode(name, nameBuf, sizeof(nameBuf) / sizeof(wchar_t));
		HANDLE file = CreateFileW(nameBuf, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
		if (file == INVALID_HANDLE_VALUE)
		{
			return returnValue;
		}
		const int size = GetFileSize(file, nullptr);
		if (size == INVALID_FILE_SIZE)
		{
			return returnValue;
		}
		HANDLE mapping = CreateFileMapping(file, nullptr, PAGE_READONLY, 0, 0, nullptr);
		if (mapping == INVALID_HANDLE_VALUE)
		{
			return returnValue;
		}
		unsigned char *data = static_cast<unsigned char *>(MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0));
		if (data == nullptr)
		{
			return returnValue;
		}
		else
		{
			returnValue.reset(new FileMap(name, data, static_cast<std::size_t>(size), file, mapping));
			return returnValue;
		}
	}
}