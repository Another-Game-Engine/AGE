#include <cassert>

#include "FileCommand.hpp"

namespace AGE
{
	FileCommand::FileCommand(const char *command, FILE *file)
		: command(std::string("command://") + command), file(file)
	{
		return;
	}

	FileCommand::~FileCommand(void)
	{
		_pclose(file);
	}

	std::shared_ptr<FileInterface> FileCommand::Create(const char *command, const char *mode)
	{
		std::shared_ptr<FileInterface> returnValue;
		assert(command != nullptr && mode != nullptr && "Invalid string");
		FILE *file = _popen(command, mode);
		if (file)
		{
			returnValue.reset(new FileCommand(command, file));
		}
		return returnValue;
	}

	const char *FileCommand::getName(void) const
	{
		return command.c_str();
	}

	std::size_t FileCommand::getSize(void)
	{
		const std::size_t offset = tell();
		seekEnd(0);
		const std::size_t size = tell();
		seekSet(static_cast<int>(offset));
		return size;
	}

	std::size_t FileCommand::read(void *p, std::size_t size, std::size_t nmemb)
	{
		return std::fread(p, size, nmemb, file);
	}

	std::size_t FileCommand::write(const void *p, std::size_t size, std::size_t nmemb)
	{
		return std::fwrite(p, size, nmemb, file);
	}

	bool FileCommand::endOfFile(void) const
	{
		return std::feof(file) == 0;
	}

	int FileCommand::getCharacter(void)
	{
		return std::fgetc(file);
	}

	void FileCommand::seekSet(int offset)
	{
		std::fseek(file, offset, SEEK_SET);
	}

	void FileCommand::seekEnd(int offset)
	{
		std::fseek(file, offset, SEEK_END);
	}

	void FileCommand::seekCurrent(int offset)
	{
		std::fseek(file, offset, SEEK_CUR);
	}

	std::size_t FileCommand::tell(void) const
	{
		return std::ftell(file);
	}

	bool FileCommand::flush(void) const
	{
		return std::fflush(file) == 0;
	}
}