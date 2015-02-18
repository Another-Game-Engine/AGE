#include <cassert>

#include "FileStdio.hpp"
#include "Encoding.hpp"

namespace AGE
{
	namespace Engine
	{
		FileStdio::FileStdio(const char *name, FILE *file)
			: name(name), file(file)
		{
			return;
		}

		FileStdio::~FileStdio(void)
		{
			fclose(file);
		}

		std::shared_ptr<FileInterface> FileStdio::Create(const char *name, const char *mode)
		{
			std::shared_ptr<FileInterface> returnValue;
			assert(name != nullptr  && mode != nullptr && "Invalid string");
			wchar_t nameWbuf[1024];
			wchar_t modeWbuf[64];
			Encoding::Utf8ToUnicode(name, nameWbuf, sizeof(nameWbuf) / sizeof(wchar_t));
			Encoding::Utf8ToUnicode(mode, modeWbuf, sizeof(modeWbuf) / sizeof(wchar_t));
			nameWbuf[sizeof(nameWbuf) / sizeof(wchar_t) - 1] = '\0';
			modeWbuf[sizeof(modeWbuf) / sizeof(wchar_t) - 1] = '\0';
			FILE *file = _wfopen(nameWbuf, modeWbuf);
			if (file)
			{
				returnValue.reset(new FileStdio(name, file));
			}
			return returnValue;
		}

		const char *FileStdio::getName(void) const
		{
			return name;
		}

		std::size_t FileStdio::getSize(void)
		{
			const std::size_t offset = tell();
			seekEnd(0);
			const std::size_t size = tell();
			seekSet(static_cast<int>(offset));
			return size;
		}

		std::size_t FileStdio::read(void *p, std::size_t size, std::size_t nmemb)
		{
			return std::fread(p, size, nmemb, file);
		}

		std::size_t FileStdio::write(const void *p, std::size_t size, std::size_t nmemb)
		{
			return std::fwrite(p, size, nmemb, file);
		}

		bool FileStdio::endOfFile(void) const
		{
			return std::feof(file) == 0;
		}

		int FileStdio::getCharacter(void)
		{
			return std::fgetc(file);
		}

		void FileStdio::seekSet(int offset)
		{
			std::fseek(file, offset, SEEK_SET);
		}

		void FileStdio::seekEnd(int offset)
		{
			std::fseek(file, offset, SEEK_END);
		}

		void FileStdio::seekCurrent(int offset)
		{
			std::fseek(file, offset, SEEK_CUR);
		}

		std::size_t FileStdio::tell(void) const
		{
			return std::ftell(file);
		}

		bool FileStdio::flush(void) const
		{
			return std::fflush(file) == 0;
		}
	}
}
