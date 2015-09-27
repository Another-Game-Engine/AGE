#include <cassert>

#include <Utils/EngineBase.hpp>
#include "File.hpp"
#include "FileInterface.hpp"
#include "FileSystem.hpp"

namespace AGE
{
	struct File::FileData final
	{
		std::shared_ptr<FileInterface> file = nullptr;
	};

	File::File(void)
		: data(new FileData)
	{
		return;
	}

	File::File(const File &other)
		: data(other.data)
	{
		return;
	}

	File::File(File &&other)
		: data(std::move(other.data))
	{
		return;
	}

	File &File::operator=(const File &other)
	{
		if (this != &other)
		{
			assert(!data->file && "File is not closed");
			data = other.data;
		}
		return *this;
	}

	File &File::operator=(File &&other)
	{
		if (this != &other)
		{
			assert(!data->file && "File is not closed");
			data = std::move(other.data);
		}
		return *this;
	}

	File::~File(void)
	{
		assert(!data->file && "File is not closed");
	}

	bool File::open(const char *name, const char *mode)
	{
		assert(!data->file && "File is already opened");
		FileSystem *fileSystem = EngineBase::g_engineInstance->getInstance<FileSystem>();
		assert(fileSystem != nullptr && "FileSystem is not initialized");
		data->file = fileSystem->getFile(name, mode);
		return data->file != nullptr;
	}

	bool File::execute(const char *command, const char *mode)
	{
		assert(!data->file && "File is already opened");
		FileSystem *fileSystem = EngineBase::g_engineInstance->getInstance<FileSystem>();
		assert(fileSystem != nullptr && "FileSystem is not initialized");
		data->file = fileSystem->getFile((std::string("command://") + command).c_str(), mode);
		return data->file != nullptr;
	}

	void File::close(void)
	{
		assert(data->file != nullptr && "File is not opened");
		data->file.reset();
		data->file = nullptr;
	}

	const char *File::getName(void) const
	{
		assert(data->file != nullptr && "File is not opened");
		return data->file->getName();
	}

	std::size_t File::getSize(void) const
	{
		assert(data->file != nullptr && "File is not opened");
		return data->file->getSize();
	}

	bool File::isOpened(void) const
	{
		return data->file != nullptr;
	}

	std::size_t File::read(void *ptr, std::size_t size, std::size_t nmemb) const
	{
		assert(data->file != nullptr && "File is not opened");
		return data->file->read(ptr, size, nmemb);
	}

	std::size_t File::write(const void *ptr, std::size_t size, std::size_t nmemb) const
	{
		assert(data->file != nullptr && "File is not opened");
		return data->file->write(ptr, size, nmemb);
	}

	bool File::endOfFile(void) const
	{
		assert(data->file != nullptr && "File is not opened");
		return data->file->endOfFile();
	}

	int File::getCharacter(void) const
	{
		assert(data->file != nullptr && "File is not opened");
		return data->file->getCharacter();
	}

	void File::seekSet(int offset) const
	{
		assert(data->file != nullptr && "File is not opened");
		return data->file->seekSet(offset);
	}

	void File::seekEnd(int offset) const
	{
		assert(data->file != nullptr && "File is not opened");
		return data->file->seekEnd(offset);
	}

	void File::seekCurrent(int offset) const
	{
		assert(data->file != nullptr && "File is not opened");
		return data->file->seekCurrent(offset);
	}

	std::size_t File::tell(void) const
	{
		assert(data->file != nullptr && "File is not opened");
		return data->file->tell();
	}

	bool File::flush(void) const
	{
		assert(data->file != nullptr && "File is not opened");
		return data->file->flush();
	}

	File &File::swap(File &other)
	{
		if (this != &other)
		{
			std::swap(data, other.data);
		}
		return *this;
	}

	void swap(File &lhs, File &rhs)
	{
		lhs.swap(rhs);
	}
}