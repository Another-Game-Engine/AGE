#include <thread>
#include <algorithm>
#include <cassert>

#include <minizip/unzip.h>

#include <Core/Engine.hh>

#include "FileSystem.hpp"
#include "Path.hpp"
#include "FileMap.hpp"
#include "FileStdio.hpp"
#include "FileCommand.hpp"
#include "FileMemory.hpp"
#include "Directory.hpp"
#include "FileSystemThread.hpp"

namespace AGE
{
	static const std::size_t FileSystemCache = 64;
	static const float FileSystemDelay = 0.0f;

	FileSystem::FileSystem(void)
		: thread(new FileSystemThread), dataICase(false)
	{
		processedFile.data = nullptr;
		processedFile.size = 0;
	}

	FileSystem::~FileSystem(void)
	{
		finalize();
		clear();
	}

	bool FileSystem::initialize(const char *p)
	{
		thread->run();
		password = p;
		std::shared_ptr<Engine> engine = Engine::GetInstance();
		dataPath = engine->getDataPath();
		// Set to true if you want to ignore case if data directory
		dataICase = false;
		relativePath = Path::RelativeName(engine->getApplicationPath().c_str(), dataPath.c_str());
		return loadDirectory(dataPath.c_str());
	}

	void FileSystem::finalize(void)
	{
		std::shared_ptr<Engine> engine = Engine::GetInstance();
		while (true)
		{
			thread->lock();
			queuedFiles.clear();
			if (processedFile.data == nullptr)
			{
				thread->stop();
				for (auto &it = loadedFiles.begin(); it != loadedFiles.end(); ++it)
				{
					delete[] it->second.data;
				}
				loadedFiles.clear();
				thread->unlock();
				break;
			}
			thread->unlock();
		}
		thread->stop();
	}


	void FileSystem::clear(void)
	{
		files.clear();
		for (std::size_t i = 0, size = zipPackages.size(); i < size; ++i)
		{
			unzClose(zipPackages[i].file);
		}
		zipPackages.clear();
	}

	void FileSystem::update(void)
	{
		std::shared_ptr<Engine> engine = Engine::GetInstance();
		bool suspend = true;
		thread->lock();
		if (!queuedFiles.empty())
		{
			std::size_t size = 0;
			FileThreadMap::iterator it;
			FileThreadMap::iterator end = loadedFiles.end();
			for (it = loadedFiles.begin(); it != end; ++it)
			{
				size += it->second.size;
			}
			if (size / 1048576 < static_cast<std::size_t>(FileSystemCache))
			{
				int priority = -1000000;
				end = queuedFiles.end();
				for (it = queuedFiles.begin(); it != end; ++it)
				{
					if (priority < it->second.priority)
					{
						priority = it->second.priority;
					}
				}
				int frame = 1000000;
				for (it = queuedFiles.begin(); it != end; ++it)
				{
					if (priority != it->second.priority)
					{
						continue;
					}
					if (frame > static_cast<int>(engine->getFrameNumber() - it->second.frame))
					{
						frame = static_cast<int>(engine->getFrameNumber() - it->second.frame);
						processedName = it->first;
					}
				}
				float weight = -1000000.0f;
				for (it = queuedFiles.begin(); it != end; ++it)
				{
					if (priority != it->second.priority)
					{
						continue;
					}
					if (frame != static_cast<int>(engine->getFrameNumber() - it->second.frame))
					{
						continue;
					}
					if (weight < it->second.weight)
					{
						weight = it->second.weight;
						processedName = it->first;
					}
				}
				it = queuedFiles.find(processedName);
				processedFile = it->second;
				queuedFiles.erase(it);
				suspend = false;
			}
		}
		thread->unlock();
		if (suspend == false)
		{
			std::size_t size = 0;
			std::shared_ptr<FileInterface> file = FileMap::Create(processedFile.name.c_str());
			if (file == nullptr)
			{
				file = FileStdio::Create(processedFile.name.c_str(), "rb");
			}
			if (file != nullptr)
			{
				thread->lock();
				processedFile.size = file->getSize();
				processedFile.data = new unsigned char[processedFile.size];
				thread->unlock();
				size = file->read(processedFile.data, sizeof(unsigned char), processedFile.size);
				file.reset();
			}
			if (FileSystemDelay > 0.0f)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(FileSystemDelay * 1000.0)));
			}
			thread->lock();
			if (size == processedFile.size)
			{
				loadedFiles.insert(std::make_pair(processedName, processedFile));
				processedName.clear();
				processedFile.name.clear();
				processedFile.data = nullptr;
				processedFile.size = 0;
			}
			else
			{
				delete[] processedFile.data;
				processedFile.name.clear();
				processedFile.data = nullptr;
				processedFile.size = 0;
				loadedFiles.insert(std::make_pair(processedName, processedFile));
				processedName.clear();
			}
			suspend = queuedFiles.empty();
			thread->unlock();
		}
		if (suspend)
		{
			std::this_thread::yield();
		}
	}

	void FileSystem::reload(void)
	{
		clear();
		loadDirectory(dataPath.c_str());
	}

	std::string FileSystem::getName(const char *name) const
	{
		std::string returnValue(name);
		if (dataICase)
		{
			std::transform(returnValue.begin(), returnValue.end(), returnValue.begin(), tolower);
		}
		return name;
	}

	bool FileSystem::loadDirectory(const char *name)
	{
		std::shared_ptr<Engine> engine = Engine::GetInstance();
		Directory directory;
		if (directory.open(name) == false)
		{
			return false;
		}
		else
		{
			const std::string &appPath = engine->getApplicationPath();
			for (std::size_t i = 0, size = directory.getNumberOfDirectories(); i < size; ++i)
			{
				const char *name = directory.getDirectoryName(i);
				if (!std::strncmp(appPath.c_str(), name, appPath.size()))
				{
					loadDirectory(name + appPath.size());
				}
				else
				{
					loadDirectory(name);
				}
			}
			for (std::size_t i = 0, size = directory.getNumberOfFiles(); i < size; ++i)
			{
				const char *name = directory.getFileName(i);
				if (!std::strncmp(appPath.c_str(), name, appPath.size()))
				{
					internalLoadFile(name + appPath.size());
				}
				else
				{
					internalLoadFile(name);
				}
			}
			directory.close();
			return true;
		}
	}

	bool FileSystem::loadFile(const char *n, int priority, float weight)
	{
		std::shared_ptr<Engine> engine = Engine::GetInstance();
		std::string name(n);
		if (name.empty())
		{
			return false;
		}
		if (dataICase)
		{
			std::transform(name.begin(), name.end(), name.begin(), tolower);
		}
		auto &it = files.find(name);
		if (it != files.end())
		{
			thread->lock();
			if (processedName == name)
			{
				thread->unlock();
				return false;
			}
			if (queuedFiles.find(name) != queuedFiles.end())
			{
				thread->unlock();
				return false;
			}
			if (loadedFiles.find(name) != loadedFiles.end())
			{
				thread->unlock();
				return false;
			}
			FileThread file;
			file.name = it->first;
			file.priority = priority;
			file.frame = engine->getFrameNumber();
			file.weight = weight;
			file.data = nullptr;
			file.size = 0;
			queuedFiles.insert(std::make_pair(name, file));
			thread->unlock();
			return true;
		}
		else
		{
			return false;
		}
	}

	bool FileSystem::internalLoadFile(const char *name)
	{
		std::string extension;
		const char *s = std::strrchr(name, '.');
		if (s)
		{
			extension = s + 1;
			std::transform(extension.begin(), extension.end(), extension.begin(), tolower);
		}
		if (extension == "zip" || extension == "pak")
		{
			zipPackages.push_back(ZipPackage());
			ZipPackage &zipPackage = zipPackages.back();
			zipPackage.path = Path::DirectoryName(name);
			zipPackage.name = name;
			zipPackage.file = unzOpen(name);
			if (zipPackage.file == nullptr)
			{
				zipPackages.pop_back();
				return false;
			}
			zipPackage.files.clear();
			int ret = unzGoToFirstFile(zipPackage.file);
			while (ret == UNZ_OK)
			{
				char name[4096];
				unz_file_info info;
				unzGetCurrentFileInfo(zipPackage.file, &info, name, sizeof(name) - 1, nullptr, 0, nullptr, 0);
				name[sizeof(name) - 1] = '\0';
				zipPackage.files.insert(std::make_pair(getName(name), static_cast<int>(zipPackage.files.size())));
				ret = unzGoToNextFile(zipPackage.file);
			}
		}
		else
		{
			if (std::strncmp(dataPath.c_str(), name, dataPath.size()))
			{
				files.insert(std::make_pair(getName(name), 0));
			}
			else
			{
				files.insert(std::make_pair(getName((relativePath + (name + dataPath.size())).c_str()), 0));
			}
		}
		return true;
	}

	std::size_t FileSystem::getNumberOfFiles(void) const
	{
		return files.size();
	}

	std::vector<std::string> FileSystem::getFileNames(void) const
	{
		std::vector<std::string> names;
		names.reserve(files.size());
		for (auto &it = files.begin(), end = files.end(); it != end; ++it)
		{
			names.push_back(it->first);
		}
		std::sort(names.begin(), names.end());
		return names;
	}

	void FileSystem::getFileNames(std::vector<std::string> &names) const
	{
		names.clear();
		names.reserve(files.size());
		for (auto &it = files.begin(), end = files.end(); it != end; ++it)
		{
			names.push_back(it->first);
		}
		std::sort(names.begin(), names.end());
	}

	std::string FileSystem::getFileName(const char *n) const
	{
		std::string name(n);
		if (name.empty())
		{
			return n;
		}
		if (dataICase)
		{
			std::transform(name.begin(), name.end(), name.begin(), tolower);
		}
		auto &it = files.find(name);
		if (it != files.end())
		{
			return it->first.c_str();
		}
		else
		{
			return n;
		}
	}

	std::size_t FileSystem::getNumberOfZipPackages(void) const
	{
		return zipPackages.size();
	}

	const std::string &FileSystem::getZipPackageName(std::size_t num) const
	{
		assert(num < zipPackages.size() && "Bad num");
		return zipPackages[num].name;
	}

	std::size_t FileSystem::getNumberOfZipPackageFiles(std::size_t num) const
	{
		assert(num < zipPackages.size() && "Bad num");
		return zipPackages[num].files.size();
	}

	std::vector<std::string> FileSystem::getZipPackageFileNames(std::size_t num) const
	{
		assert(num < zipPackages.size() && "Bad num");
		std::vector<std::string> names;
		names.reserve(zipPackages[num].files.size());
		auto &end = zipPackages[num].files.end();
		for (auto &it = zipPackages[num].files.begin(); it != end; ++it)
		{
			names.push_back(it->first);
		}
		std::sort(names.begin(), names.end());
		return names;
	}

	void FileSystem::getZipPackageFileNames(std::size_t num, std::vector<std::string> &names) const
	{
		assert(num < zipPackages.size() && "Bad num");
		names.clear();
		names.reserve(zipPackages[num].files.size());
		auto &end = zipPackages[num].files.end();
		for (auto &it = zipPackages[num].files.begin(); it != end; ++it)
		{
			names.push_back(it->first);
		}
		std::sort(names.begin(), names.end());
	}

	std::shared_ptr<FileInterface> FileSystem::getFile(const char *n, const char *mode)
	{
		std::shared_ptr<Engine> engine = Engine::GetInstance();
		std::string name = std::string(n);
		if (name.empty())
		{
			return std::shared_ptr<FileInterface>();
		}
		if (dataICase)
		{
			std::transform(name.begin(), name.end(), name.begin(), tolower);
		}
		if (name.find("command://") == 0)
		{
			return FileCommand::Create(name.substr(sizeof("command://") - 1).c_str(), mode);
		}
		if (std::strchr(mode, 'w') || std::strchr(mode, '+'))
		{
			std::shared_ptr<FileInterface> file;
			auto &it = files.find(name);
			if (it != files.end())
			{
				file = FileStdio::Create(it->first.c_str(), mode);
				if (file)
				{
					return file;
				}
			}
			file = FileStdio::Create((engine->getApplicationPath() + name).c_str(), mode);
			if (file)
			{
				files.insert(std::make_pair(name, 0));
				return file;
			}
			file = FileStdio::Create((engine->getDataPath() + name).c_str(), mode);
			if (file)
			{
				files.insert(std::make_pair(engine->getDataPath() + name, 0));
				return file;
			}
			file = FileStdio::Create(name.c_str(), mode);
			return file;
		}
		if (thread)
		{
			thread->lock();
			auto &it = loadedFiles.find(name);
			if (it != loadedFiles.end())
			{
				std::shared_ptr<FileInterface> file = std::make_shared<FileMemory>(name.c_str(), it->second.data, it->second.size, true);
				loadedFiles.erase(it);
				thread->unlock();
				return file;
			}
			thread->unlock();
		}
		std::shared_ptr<FileInterface> file = FileMap::Create(name.c_str());
		if (file)
		{
			return file;
		}
		file = FileMap::Create((relativePath + name).c_str());
		if (file)
		{
			return file;
		}
		auto &it = files.find(name);
		if (it != files.end())
		{
			file = FileMap::Create(it->first.c_str());
			if (file)
			{
				return file;
			}
			file = FileStdio::Create(it->first.c_str(), "rb");
			if (file)
			{
				return file;
			}
		}
		for (std::size_t i = 0, packageSize = zipPackages.size(); i < packageSize; ++i)
		{
			ZipPackage &zipPackage = zipPackages[i];
			auto &it = zipPackage.files.find(name);
			if (it == zipPackage.files.end())
			{
				continue;
			}
			thread->lock();
			int ret = unzGoToFirstFile(zipPackage.file);
			for (int k = 0, max = it->second; k < max && ret == UNZ_OK; ++k)
			{
				ret = unzGoToNextFile(zipPackage.file);
			}
			if (ret != UNZ_OK)
			{
				thread->unlock();
				return std::shared_ptr<FileInterface>();
			}
			if (password.empty() ? unzOpenCurrentFile(zipPackage.file) != UNZ_OK : unzOpenCurrentFilePassword(zipPackage.file, password.c_str()) != UNZ_OK)
			{
				thread->unlock();
				return std::shared_ptr<FileInterface>();
			}
			unz_file_info info;
			unzGetCurrentFileInfo(zipPackage.file, &info, nullptr, 0, nullptr, 0, nullptr, 0);
			uLong size = info.uncompressed_size;
			unsigned char *data = new unsigned char[size + 1];
			unzReadCurrentFile(zipPackage.file, data, size);
			unzCloseCurrentFile(zipPackage.file);
			data[size] = '\0';
			thread->unlock();
			return std::make_shared<FileMemory>(name.c_str(), data, size, true);
		}
		return std::shared_ptr<FileInterface>();
	}

	bool FileSystem::forceFile(const char *n)
	{
		std::shared_ptr<Engine> engine = Engine::GetInstance();
		std::string name(n);
		if (name.empty())
		{
			return false;
		}
		if (dataICase)
		{
			std::transform(name.begin(), name.end(), name.begin(), tolower);
		}
		thread->lock();
		auto &it = queuedFiles.find(name);
		if (it != queuedFiles.end())
		{
			it->second.priority = 1000000;
			it->second.frame = engine->getFrameNumber();
			thread->unlock();
			return true;
		}
		thread->unlock();
		return false;
	}

	bool FileSystem::removeFile(const char *n)
	{
		std::shared_ptr<Engine> engine = Engine::GetInstance();
		std::string name(n);
		if (name.empty())
		{
			return false;
		}
		if (dataICase)
		{
			std::transform(name.begin(), name.end(), name.begin(), tolower);
		}
		thread->lock();
		if (processedName == name)
		{
			thread->unlock();
			while (validateFile(n) == false)
			{
				std::this_thread::yield();
			}
			return removeFile(n);
		}
		auto &it = queuedFiles.find(name);
		if (it != queuedFiles.end())
		{
			queuedFiles.erase(it);
			thread->unlock();
			return true;
		}
		it = loadedFiles.find(name);
		if (it != loadedFiles.end())
		{
			delete[] it->second.data;
			loadedFiles.erase(it);
			thread->unlock();
			return true;
		}
		thread->unlock();
		return false;
	}

	bool FileSystem::checkFile(const char *n)
	{
		std::shared_ptr<Engine> engine = Engine::GetInstance();
		std::string name(n);
		if (name.empty())
		{
			return false;
		}
		if (dataICase)
		{
			std::transform(name.begin(), name.end(), name.begin(), tolower);
		}
		thread->lock();
		if (processedName == name)
		{
			thread->unlock();
			return true;
		}
		auto &it = queuedFiles.find(name);
		if (it != queuedFiles.end())
		{
			it->second.frame = engine->getFrameNumber();
			thread->unlock();
			return true;
		}
		if (loadedFiles.find(name) != loadedFiles.end())
		{
			thread->unlock();
			return true;
		}
		thread->unlock();
		return false;
	}

	bool FileSystem::validateFile(const char *n) const
	{
		std::shared_ptr<Engine> engine = Engine::GetInstance();
		std::string name(n);
		if (name.empty())
		{
			return false;
		}
		if (dataICase)
		{
			std::transform(name.begin(), name.end(), name.begin(), tolower);
		}
		thread->lock();
		if (loadedFiles.find(name) != loadedFiles.end())
		{
			thread->unlock();
			return true;
		}
		thread->unlock();
		return false;
	}

	long long FileSystem::getLastModificationTime(const char *n) const
	{
		std::string name(n);
		if (name.empty())
		{
			return -1;
		}
		if (dataICase)
		{
			std::transform(name.begin(), name.end(), name.begin(), tolower);
		}
		long long mtime = Directory::GetLastModificationTime(name.c_str());
		if (mtime != -1)
		{
			return mtime;
		}
		else
		{
			auto &it = files.find(name);
			if (it != files.end())
			{
				mtime = Directory::GetLastModificationTime(it->first.c_str());
				if (mtime != -1)
				{
					return mtime;
				}
				else
				{
					return -1;
				}
			}

			return -1;
		}
	}

	const char *FileSystem::getLastModificationDate(const char *name) const
	{
		long long timestamp = getLastModificationTime(name);
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

	std::shared_ptr<FileSystemThread> FileSystem::getThread(void) const
	{
		return thread;
	}
}