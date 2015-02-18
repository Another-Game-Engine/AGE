#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "File.hpp"
#include "FileInterface.hpp"

namespace AGE
{
	namespace Engine
	{
		class FileSystemThread;

		class FileSystem final
		{
		public:
			typedef std::unordered_map<std::string, int> FileSystemMap;

			FileSystem(void);
			FileSystem(const FileSystem &other) = delete;
			FileSystem &operator=(const FileSystem &other) = delete;
			~FileSystem(void);

			bool initialize(const char *password);
			void finalize(void);
			void clear(void);
			void update(void);
			std::size_t getNumberOfFiles(void) const;
			std::vector<std::string> getFileNames(void) const;
			void getFileNames(std::vector<std::string> &names) const;
			std::string getFileName(const char *name) const;
			std::size_t getNumberOfZipPackages(void) const;
			const std::string &getZipPackageName(std::size_t num) const;
			std::size_t getNumberOfZipPackageFiles(std::size_t num) const;
			std::vector<std::string> getZipPackageFileNames(std::size_t num) const;
			void getZipPackageFileNames(std::size_t num, std::vector<std::string> &names) const;
			std::shared_ptr<FileInterface> getFile(const char *name, const char *mode);
			bool loadFile(const char *name, int priority = 0, float weight = 0.0f);
			bool forceFile(const char *name);
			bool removeFile(const char *name);
			bool checkFile(const char *name);
			bool validateFile(const char *name) const;
			const char *getLastModificationDate(const char *name) const;
			long long getLastModificationTime(const char *name) const;
			std::shared_ptr<FileSystemThread> getThread(void) const;

		private:
			std::shared_ptr<FileSystemThread> thread;
			std::string password;
			std::string dataPath;
			bool dataICase;
			std::string relativePath;

			struct ZipPackage
			{
				std::string path;
				std::string name;
				void *file;
				FileSystemMap files;
			};

			FileSystemMap files;
			std::vector<ZipPackage> zipPackages;

			struct FileThread
			{
				std::string name;
				int priority;
				std::size_t frame;
				float weight;
				unsigned char *data;
				std::size_t size;
			};

			typedef std::unordered_map<std::string, FileThread> FileThreadMap;

			std::string processedName;
			FileThread processedFile;
			FileThreadMap queuedFiles;
			FileThreadMap loadedFiles;

			void reload(void);
			std::string getName(const char *name) const;
			bool loadDirectory(const char *name);
			bool internalLoadFile(const char *name);
		};
	}
}