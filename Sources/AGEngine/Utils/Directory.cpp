#include <sys/stat.h>
#include <direct.h>
#include <io.h>
#include <cassert>
#include <ctime>

#include "Platform.hpp"
#include "Directory.hpp"
#include "File.hpp"

#if defined(GetCurrentDirectory)
# undef GetCurrentDirectory
#endif

#if defined(CreateDirectory)
# undef CreateDirectory
#endif

#if defined(RemoveDirectory)
# undef RemoveDirectory
#endif

#if defined(CreateFile)
# undef CreateFile
#endif

namespace AGE
{
	struct Directory::DirData final
	{
		char *name = nullptr;
		std::size_t numberOfDirectories = 0;
		char **directories = nullptr;
		std::size_t numberOfFiles = 0;
		char **files = nullptr;
	};

	Directory::Directory(void)
		: data(new DirData)
	{
		return;
	}

	Directory::Directory(const Directory &other)
		: data(other.data)
	{
		return;
	}

	Directory::Directory(Directory &&other)
		: data(std::move(other.data))
	{
		return;
	}

	Directory &Directory::operator=(const Directory &other)
	{
		if (this != &other)
		{
			assert(!data->name && "Directory is not closed");
			data = other.data;
		}
		return *this;
	}

	Directory &Directory::operator=(Directory &&other)
	{
		if (this != &other)
		{
			assert(!data->name && "Directory is not closed");
			data = std::move(other.data);
		}
		return *this;
	}

	Directory::~Directory(void)
	{
		assert(!data->name && "Directory is not closed");
	}

	Directory &Directory::swap(Directory &other)
	{
		std::swap(data, other.data);
		return *this;
	}

	void Directory::addDirectory(const char *name)
	{
		assert(name != nullptr && "Invalid name");
		if (data->numberOfDirectories % MaximumNumberOfFiles == 0)
		{
			char **directories = data->directories;
			data->directories = new char *[data->numberOfDirectories + MaximumNumberOfFiles];
			for (std::size_t i = 0; i < data->numberOfDirectories; ++i)
			{
				data->directories[i] = directories[i];
			}
			delete[] directories;
		}
		data->directories[data->numberOfDirectories] = new char[std::strlen(name) + 1];
		std::strcpy(data->directories[data->numberOfDirectories], name);
		++data->numberOfDirectories;
	}

	void Directory::addFile(const char *name)
	{
		assert(name != nullptr && "Invalid name");
		if (data->numberOfFiles % MaximumNumberOfFiles == 0)
		{
			char **files = data->files;
			data->files = new char *[data->numberOfFiles + MaximumNumberOfFiles];
			for (std::size_t i = 0; i < data->numberOfFiles; ++i)
			{
				data->files[i] = files[i];
			}
			if (data->numberOfFiles)
				delete[] files;
		}
		data->files[data->numberOfFiles] = new char[std::strlen(name) + 1];
		std::strcpy(data->files[data->numberOfFiles], name);
		++data->numberOfFiles;
	}

	bool Directory::open(const char *name)
	{
		assert(!data->name && "Directory is already opened");
		assert(name != nullptr && "Invalid name");
		char buf[BufferSize];
		char file[BufferSize];
		wchar_t wbuf[BufferSize];
		const bool shouldAddSlash = name[std::strlen(name) - 1] != '/';
		shouldAddSlash ? _snprintf(buf, sizeof(buf), "%s/*", name) : _snprintf(buf, sizeof(buf), "%s*", name);
		buf[sizeof(buf) - 1] = '\0';
		MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, sizeof(wbuf) / sizeof(wchar_t));
		wbuf[sizeof(wbuf) / sizeof(wchar_t) - 1] = '\0';
		struct _wfinddata_t fileinfo;
		intptr_t handle = _wfindfirst(wbuf, &fileinfo);
		if (handle == -1)
		{
			return false;
		}
		do
		{
			if (!wcscmp(fileinfo.name, L".") || !wcscmp(fileinfo.name, L".."))
			{
				continue;
			}
			WideCharToMultiByte(CP_UTF8, 0, fileinfo.name, -1, file, sizeof(file), nullptr, nullptr);
			file[sizeof(file) - 1] = '\0';
			shouldAddSlash ? _snprintf(buf, sizeof(buf), "%s/%s", name, file) : _snprintf(buf, sizeof(buf), "%s%s", name, file);
			buf[sizeof(buf) - 1] = '\0';
			if (fileinfo.attrib & _A_SUBDIR)
			{
				std::strcat(buf, "/");
				addDirectory(buf);
			}
			else
			{
				addFile(buf);
			}
		} while (_wfindnext(handle, &fileinfo) != -1);
		_findclose(handle);
		data->name = new char[std::strlen(name) + 1];
		std::strcpy(data->name, name);
		return data->name != nullptr;
	}

	void Directory::close(void)
	{
		assert(data->name != nullptr && "Directory is not opened");
		delete[] data->name;
		data->name = nullptr;
		for (std::size_t i = 0; i < data->numberOfDirectories; ++i)
		{
			delete[] data->directories[i];
		}
		if (data->numberOfDirectories)
			delete[] data->directories;
		data->numberOfDirectories = 0;
		data->directories = nullptr;
		for (std::size_t i = 0; i < data->numberOfFiles; ++i)
		{
			delete[] data->files[i];
		}
		if (data->numberOfFiles)
			delete[] data->files;
		data->numberOfFiles = 0;
		data->files = nullptr;
	}

	bool Directory::isOpened(void) const
	{
		return data->name != nullptr;
	}

	const char *Directory::getName(void) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		return data->name;
	}

	std::size_t Directory::getNumberOfDirectories(void) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		return data->numberOfDirectories;
	}

	const char *Directory::getDirectoryName(std::size_t num) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		assert(num < data->numberOfDirectories && "Bad directory number");
		return data->directories[num];
	}

	std::size_t Directory::getNumberOfFiles(void) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		return data->numberOfFiles;
	}

	const char *Directory::getFileName(std::size_t num) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		assert(num < data->numberOfFiles && "Bad file number");
		return data->files[num];
	}

	const char *Directory::GetHomeDirectory(void)
	{
		static char buf[BufferSize];
		wchar_t *wbuf = _wgetenv(L"USERPROFILE");
		WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, buf, sizeof(buf), nullptr, nullptr);
		if (buf[0])
		{
			buf[0] = toupper(buf[0]);
		}
		buf[sizeof(buf) - 1] = '\0';
		for (char *s = buf; *s != '\0'; ++s)
		{
			if (*s == '\\')
			{
				*s = '/';
			}
		}
		return buf;
	}

	const char *Directory::GetCurrentDirectory(void)
	{
		static char buf[BufferSize];
		static wchar_t wbuf[BufferSize];
		_wgetcwd(wbuf, sizeof(wbuf) / sizeof(wchar_t));
		wbuf[sizeof(wbuf) / sizeof(wchar_t) - 1] = '\0';
		WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, buf, sizeof(buf), nullptr, nullptr);
		if (buf[0]) buf[0] = toupper(buf[0]);
		buf[sizeof(buf) - 1] = '\0';
		for (char *s = buf; *s != '\0'; s++)
		{
			if (*s == '\\')
			{
				*s = '/';
			}
		}
		std::strcat(buf, "/");
		return buf;
	}

	bool Directory::ChangeDirectory(const char *name)
	{
		assert(name != nullptr && "Invalid name");
		wchar_t wbuf[BufferSize];
		MultiByteToWideChar(CP_UTF8, 0, name, -1, wbuf, sizeof(wbuf) / sizeof(wchar_t));
		wbuf[sizeof(wbuf) / sizeof(wchar_t) - 1] = '\0';
		return _wchdir(wbuf) == 0;
	}

	bool Directory::CreateDirectory(const char *name)
	{
		assert(name != nullptr && "Invalid name");
		wchar_t wbuf[BufferSize];
		MultiByteToWideChar(CP_UTF8, 0, name, -1, wbuf, sizeof(wbuf) / sizeof(wchar_t));
		wbuf[sizeof(wbuf) / sizeof(wchar_t) - 1] = '\0';
		return _wmkdir(wbuf) == 0;
	}

	bool Directory::CreateFile(const char *name)
	{
		assert(name != nullptr && "Invalid name");
		File f;
		f.open(name, "w");
		if (f.isOpened())
		{
			f.close();
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Directory::RemoveDirectory(const char *name)
	{
		assert(name != nullptr && "Invalid name");
		wchar_t wbuf[BufferSize];
		MultiByteToWideChar(CP_UTF8, 0, name, -1, wbuf, sizeof(wbuf) / sizeof(wchar_t));
		wbuf[sizeof(wbuf) / sizeof(wchar_t) - 1] = '\0';
		return _wrmdir(wbuf) == 0;
	}

	bool Directory::RemoveFile(const char *name)
	{
		assert(name != nullptr && "Invalid name");
		wchar_t wbuf[BufferSize];
		MultiByteToWideChar(CP_UTF8, 0, name, -1, wbuf, sizeof(wbuf) / sizeof(wchar_t));
		wbuf[sizeof(wbuf) / sizeof(wchar_t) - 1] = '\0';
		return _wremove(wbuf) == 0;
	}

	bool Directory::Rename(const char *name, const char *newName)
	{
		assert(name != nullptr && newName != nullptr && "Invalid name");
		wchar_t wbuf[BufferSize];
		wchar_t new_wbuf[BufferSize];
		MultiByteToWideChar(CP_UTF8, 0, name, -1, wbuf, sizeof(wbuf) / sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, newName, -1, new_wbuf, sizeof(new_wbuf) / sizeof(wchar_t));
		wbuf[sizeof(wbuf) / sizeof(wchar_t) - 1] = '\0';
		new_wbuf[sizeof(new_wbuf) / sizeof(wchar_t) - 1] = '\0';
		return _wrename(wbuf, new_wbuf) == 0;
	}

	bool Directory::IsDirectory(const char *name)
	{
		assert(name != nullptr && "Invalid name");
		struct _stat st;
		wchar_t buf[BufferSize];
		MultiByteToWideChar(CP_UTF8, 0, name, -1, buf, sizeof(buf) / sizeof(wchar_t));
		buf[sizeof(buf) / sizeof(wchar_t) - 1] = '\0';
		size_t length = wcslen(buf);
		if (length > 0 && (buf[length - 1] == '/' || buf[length - 1] == '\\'))
		{
			buf[length - 1] = '\0';
		}
		if (_wstat(buf, &st))
		{
			return false;
		}
		else if (st.st_mode & _S_IFDIR)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Directory::IsFile(const char *name)
	{
		assert(name != nullptr && "Invalid name");
		struct _stat st;
		wchar_t buf[BufferSize];
		MultiByteToWideChar(CP_UTF8, 0, name, -1, buf, sizeof(buf) / sizeof(wchar_t));
		buf[sizeof(buf) / sizeof(wchar_t) - 1] = '\0';
		if (_wstat(buf, &st))
		{
			return false;
		}
		else if (st.st_mode & _S_IFREG)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	long long Directory::GetLastModificationTime(const char *name)
	{
		assert(name != nullptr && "Invalid name");
		struct _stat st;
		wchar_t buf[BufferSize];
		MultiByteToWideChar(CP_UTF8, 0, name, -1, buf, sizeof(buf) / sizeof(wchar_t));
		buf[sizeof(buf) / sizeof(wchar_t) - 1] = '\0';
		if (_wstat(buf, &st))
		{
			return -1;
		}
		else
		{
			return static_cast<long long>(st.st_mtime);
		}
	}

	const char *Directory::GetLastModificationDate(const char *name)
	{
		long long timestamp = Directory::GetLastModificationTime(name);
		if (timestamp != -1)
		{
			struct _stat st;
			static_cast<void>(st);
			auto time = static_cast<decltype(st.st_mtime)>(timestamp);
			return ctime(&time);
		}
		else
		{
			return "";
		}
	}

	Directory::file_iterator::file_iterator(std::shared_ptr<DirData> data, std::size_t index)
		: data(data), index(index)
	{
		return;
	}

	Directory::file_iterator::file_iterator(void)
		: data(), index(0)
	{
		return;
	}

	Directory::file_iterator::file_iterator(const file_iterator &it)
		: data(it.data), index(it.index)
	{
		return;
	}

	Directory::file_iterator::file_iterator(file_iterator &&it)
		: data(std::move(it.data)), index(it.index)
	{
		it.index = 0;
	}

	Directory::file_iterator &Directory::file_iterator::operator=(const file_iterator &it)
	{
		if (this != &it)
		{
			data = it.data;
			index = it.index;
		}
		return *this;
	}

	Directory::file_iterator &Directory::file_iterator::operator=(file_iterator &&it)
	{
		if (this != &it)
		{
			data = std::move(it.data);
			index = it.index;
			it.index = 0;
		}
		return *this;
	}

	Directory::file_iterator::~file_iterator(void)
	{
		return;
	}

	bool Directory::file_iterator::operator==(const file_iterator &it) const
	{
		return data == it.data && index == it.index;
	}

	bool Directory::file_iterator::operator!=(const file_iterator &it) const
	{
		return data != it.data || index != it.index;
	}

	char *Directory::file_iterator::operator*(void)
	{
		return data->files[index];
	}

	const char *Directory::file_iterator::operator*(void) const
	{
		return data->files[index];
	}

	char &Directory::file_iterator::operator->(void)
	{
		return *data->files[index];
	}

	const char &Directory::file_iterator::operator->(void) const
	{
		return *data->files[index];
	}

	char *Directory::file_iterator::get(void)
	{
		return data->files[index];
	}

	const char *Directory::file_iterator::get(void) const
	{
		return data->files[index];
	}

	Directory::file_iterator &Directory::file_iterator::operator++(void)
	{
		++index;
		return *this;
	}

	Directory::file_iterator Directory::file_iterator::operator++(int)
	{
		file_iterator tmp = *this;
		++index;
		return tmp;
	}

	Directory::directory_iterator::directory_iterator(std::shared_ptr<DirData> data, std::size_t index)
		: data(data), index(index)
	{
		return;
	}

	Directory::directory_iterator::directory_iterator(void)
		: data(), index(0)
	{
		return;
	}

	Directory::directory_iterator::directory_iterator(const directory_iterator &it)
		: data(it.data), index(it.index)
	{
		return;
	}

	Directory::directory_iterator::directory_iterator(directory_iterator &&it)
		: data(std::move(it.data)), index(it.index)
	{
		it.index = 0;
	}

	Directory::directory_iterator &Directory::directory_iterator::operator=(const directory_iterator &it)
	{
		if (this != &it)
		{
			data = it.data;
			index = it.index;
		}
		return *this;
	}

	Directory::directory_iterator &Directory::directory_iterator::operator=(directory_iterator &&it)
	{
		if (this != &it)
		{
			data = std::move(it.data);
			index = it.index;
			it.index = 0;
		}
		return *this;
	}

	Directory::directory_iterator::~directory_iterator(void)
	{
		return;
	}

	bool Directory::directory_iterator::operator==(const directory_iterator &it) const
	{
		return data == it.data && index == it.index;
	}

	bool Directory::directory_iterator::operator!=(const directory_iterator &it) const
	{
		return data != it.data || index != it.index;
	}

	char *Directory::directory_iterator::operator*(void)
	{
		return data->directories[index];
	}

	const char *Directory::directory_iterator::operator*(void) const
	{
		return data->directories[index];
	}

	char &Directory::directory_iterator::operator->(void)
	{
		return *data->directories[index];
	}

	const char &Directory::directory_iterator::operator->(void) const
	{
		return *data->directories[index];
	}

	char *Directory::directory_iterator::get(void)
	{
		return data->directories[index];
	}

	const char *Directory::directory_iterator::get(void) const
	{
		return data->directories[index];
	}

	Directory::directory_iterator &Directory::directory_iterator::operator++(void)
	{
		++index;
		return *this;
	}

	Directory::directory_iterator Directory::directory_iterator::operator++(int)
	{
		directory_iterator tmp = *this;
		++index;
		return tmp;
	}

	Directory::iterator::iterator(std::shared_ptr<DirData> data, std::size_t index)
		: data(data), index(index)
	{
		return;
	}

	Directory::iterator::iterator(void)
		: data(), index(0)
	{
		return;
	}

	Directory::iterator::iterator(const iterator &it)
		: data(it.data), index(it.index)
	{
		return;
	}

	Directory::iterator::iterator(iterator &&it)
		: data(std::move(it.data)), index(it.index)
	{
		it.index = 0;
	}

	Directory::iterator &Directory::iterator::operator=(const iterator &it)
	{
		if (this != &it)
		{
			data = it.data;
			index = it.index;
		}
		return *this;
	}

	Directory::iterator &Directory::iterator::operator=(iterator &&it)
	{
		if (this != &it)
		{
			data = std::move(it.data);
			index = it.index;
			it.index = 0;
		}
		return *this;
	}

	Directory::iterator::~iterator(void)
	{
		return;
	}

	bool Directory::iterator::operator==(const iterator &it) const
	{
		return data == it.data && index == it.index;
	}

	bool Directory::iterator::operator!=(const iterator &it) const
	{
		return data != it.data || index != it.index;
	}

	char *Directory::iterator::operator*(void)
	{
		if (index < data->numberOfDirectories)
		{
			return data->directories[index];
		}
		else
		{
			return data->files[index - data->numberOfDirectories];
		}
	}

	const char *Directory::iterator::operator*(void) const
	{
		if (index < data->numberOfDirectories)
		{
			return data->directories[index];
		}
		else
		{
			return data->files[index - data->numberOfDirectories];
		}
	}

	char &Directory::iterator::operator->(void)
	{
		if (index < data->numberOfDirectories)
		{
			return *data->directories[index];
		}
		else
		{
			return *data->files[index - data->numberOfDirectories];
		}
	}

	const char &Directory::iterator::operator->(void) const
	{
		if (index < data->numberOfDirectories)
		{
			return *data->directories[index];
		}
		else
		{
			return *data->files[index - data->numberOfDirectories];
		}
	}

	char *Directory::iterator::get(void)
	{
		if (index < data->numberOfDirectories)
		{
			return data->directories[index];
		}
		else
		{
			return data->files[index - data->numberOfDirectories];
		}
	}

	const char *Directory::iterator::get(void) const
	{
		if (index < data->numberOfDirectories)
		{
			return data->directories[index];
		}
		else
		{
			return data->files[index - data->numberOfDirectories];
		}
	}

	Directory::iterator &Directory::iterator::operator++(void)
	{
		++index;
		return *this;
	}

	Directory::iterator Directory::iterator::operator++(int)
	{
		iterator tmp = *this;
		++index;
		return tmp;
	}

	Directory::recursive_iterator::recursive_iterator(std::shared_ptr<DirData> data, std::size_t index)
		: datas()
	{
		datas.push(StackData(data, index));
	}

	Directory::recursive_iterator::recursive_iterator(void)
		: datas()
	{
		datas.push(StackData(std::shared_ptr<DirData>(), 0));
	}

	Directory::recursive_iterator::recursive_iterator(const recursive_iterator &other)
		: datas(other.datas)
	{
		return;
	}

	Directory::recursive_iterator::recursive_iterator(recursive_iterator &&other)
		: datas(std::move(other.datas))
	{
		return;
	}

	Directory::recursive_iterator &Directory::recursive_iterator::operator=(const recursive_iterator &it)
	{
		if (this != &it)
		{
			datas = it.datas;
		}
		return *this;
	}

	Directory::recursive_iterator &Directory::recursive_iterator::operator=(recursive_iterator &&it)
	{
		if (this != &it)
		{
			datas = std::move(it.datas);
		}
		return *this;
	}

	Directory::recursive_iterator::~recursive_iterator(void)
	{
		return;
	}

	bool Directory::recursive_iterator::operator==(const recursive_iterator &it) const
	{
		return datas.top() == it.datas.top() && datas.size() == it.datas.size();
	}

	bool Directory::recursive_iterator::operator!=(const recursive_iterator &it) const
	{
		return datas.top() != it.datas.top() || datas.size() != it.datas.size();
	}

	char *Directory::recursive_iterator::operator*(void)
	{
		const StackData &data = datas.top();
		if (data.index < data.data->numberOfDirectories)
		{
			return data.data->directories[data.index];
		}
		else
		{
			return data.data->files[data.index - data.data->numberOfDirectories];
		}
	}

	const char *Directory::recursive_iterator::operator*(void) const
	{
		const StackData &data = datas.top();
		if (data.index < data.data->numberOfDirectories)
		{
			return data.data->directories[data.index];
		}
		else
		{
			return data.data->files[data.index - data.data->numberOfDirectories];
		}
	}

	char &Directory::recursive_iterator::operator->(void)
	{
		const StackData &data = datas.top();
		if (data.index < data.data->numberOfDirectories)
		{
			return *data.data->directories[data.index];
		}
		else
		{
			return *data.data->files[data.index - data.data->numberOfDirectories];
		}
	}

	const char &Directory::recursive_iterator::operator->(void) const
	{
		const StackData &data = datas.top();
		if (data.index < data.data->numberOfDirectories)
		{
			return *data.data->directories[data.index];
		}
		else
		{
			return *data.data->files[data.index - data.data->numberOfDirectories];
		}
	}

	char *Directory::recursive_iterator::get(void)
	{
		const StackData &data = datas.top();
		if (data.index < data.data->numberOfDirectories)
		{
			return data.data->directories[data.index];
		}
		else
		{
			return data.data->files[data.index - data.data->numberOfDirectories];
		}
	}

	const char *Directory::recursive_iterator::get(void) const
	{
		const StackData &data = datas.top();
		if (data.index < data.data->numberOfDirectories)
		{
			return data.data->directories[data.index];
		}
		else
		{
			return data.data->files[data.index - data.data->numberOfDirectories];
		}
	}

	Directory::recursive_iterator &Directory::recursive_iterator::operator++(void)
	{
		StackData &data = datas.top();
		if (data.dir->isOpened())
		{
			data.dir->close();
		}
		if (data.index < data.data->numberOfDirectories)
		{
			if (data.dir->open(data.data->directories[data.index]) && data.dir->getNumberOfDirectories() + data.dir->getNumberOfFiles() > 0)
			{
				datas.push(StackData(data.dir->data, 0));
			}
			++data.index;
		}
		else if (data.index < data.data->numberOfDirectories + data.data->numberOfFiles - 1)
		{
			++data.index;
		}
		else if (datas.size() > 1)
		{
			datas.pop();
			while (datas.size() > 1)
			{
				StackData &data = datas.top();
				if (data.index >= data.data->numberOfDirectories + data.data->numberOfFiles)
				{
					datas.pop();
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			++data.index;
		}
		return *this;
	}

	Directory::recursive_iterator Directory::recursive_iterator::operator++(int)
	{
		recursive_iterator tmp(*this);
		++*this;
		return tmp;
	}

	Directory::directory_iterator Directory::directory_begin(void) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		return directory_iterator(data, 0);
	}

	Directory::directory_iterator Directory::directory_end(void) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		return directory_iterator(data, data->numberOfDirectories);
	}

	Directory::file_iterator Directory::file_begin(void) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		return file_iterator(data, 0);
	}

	Directory::file_iterator Directory::file_end(void) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		return file_iterator(data, data->numberOfFiles);
	}

	Directory::iterator Directory::begin(void) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		return iterator(data, 0);
	}

	Directory::iterator Directory::end(void) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		return iterator(data, data->numberOfDirectories + data->numberOfFiles);
	}

	Directory::recursive_iterator Directory::recursive_begin(void) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		return recursive_iterator(data, 0);
	}

	Directory::recursive_iterator Directory::recursive_end(void) const
	{
		assert(data->name != nullptr && "Directory is not opened");
		return recursive_iterator(data, data->numberOfDirectories + data->numberOfFiles);
	}

	Directory::recursive_iterator::StackData::StackData(void)
		: data(), index(0), dir()
	{
		return;
	}

	Directory::recursive_iterator::StackData::StackData(std::shared_ptr<DirData> data, std::size_t index)
		: data(data), index(index), dir(new Directory)
	{
		return;
	}

	Directory::recursive_iterator::StackData &Directory::recursive_iterator::StackData::operator=(const StackData &other)
	{
		if (this != &other)
		{
			data = other.data;
			index = other.index;
			if (dir.use_count() == 1 && dir->isOpened())
			{
				dir->close();
			}
			dir = other.dir;
		}
		return *this;
	}

	Directory::recursive_iterator::StackData &Directory::recursive_iterator::StackData::operator=(StackData &&other)
	{
		if (this != &other)
		{
			data = std::move(other.data);
			index = other.index;
			dir = std::move(other.dir);
			other.index = 0;
		}
		return *this;
	}

	Directory::recursive_iterator::StackData::~StackData(void)
	{
		if (dir.use_count() == 1 && dir->isOpened())
		{
			dir->close();
		}
	}

	bool Directory::recursive_iterator::StackData::operator==(const StackData &other) const
	{
		return data == other.data && index == other.index;
	}

	bool Directory::recursive_iterator::StackData::operator!=(const StackData &other) const
	{
		return data != other.data || index != other.index;
	}
}