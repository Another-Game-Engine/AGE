#pragma once

#include <cstddef>
#include <stack>
#include <memory>

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

namespace Age
{
	namespace Engine
	{
		class Directory
		{
		public:
			Directory(void);
			Directory(const Directory &other);
			Directory(Directory &&other);
			Directory &operator=(const Directory &other);
			Directory &operator=(Directory &&other);
			~Directory(void);

			Directory &swap(Directory &other);

			bool open(const char *name);
			void close(void);
			bool isOpened(void) const;
			const char *getName(void) const;
			std::size_t getNumberOfDirectories(void) const;
			const char *getDirectoryName(std::size_t num) const;
			std::size_t getNumberOfFiles(void) const;
			const char *getFileName(std::size_t num) const;

			static const char *GetHomeDirectory(void);
			static const char *GetCurrentDirectory(void);
			static bool ChangeDirectory(const char *name);
			static bool CreateDirectory(const char *name);
			static bool CreateFile(const char *name);
			static bool RemoveDirectory(const char *name);
			static bool RemoveFile(const char *name);
			static bool Rename(const char *name, const char *newName);
			static bool IsDirectory(const char *name);
			static bool IsFile(const char *name);
			static long long GetLastModificationTime(const char *name);
			static const char *GetLastModificationDate(const char *name);

		private:
			static const std::size_t MaximumNumberOfFiles = 256;
			static const std::size_t BufferSize = 1024;

			struct DirData;
			std::shared_ptr<DirData> data;

			void addDirectory(const char *name);
			void addFile(const char *name);

		public:
			class file_iterator
			{
			private:
				friend class Directory;

				file_iterator(std::shared_ptr<DirData> data, std::size_t index);

			public:
				typedef const char *DataType;

				file_iterator(void);
				file_iterator(const file_iterator &it);
				file_iterator(file_iterator &&it);
				file_iterator &operator=(const file_iterator &it);
				file_iterator &operator=(file_iterator &&it);
				~file_iterator(void);

				bool operator==(const file_iterator &it) const;
				bool operator!=(const file_iterator &it) const;

				char *operator*(void);
				const char *operator*(void) const;
				char &operator->(void);
				const char &operator->(void) const;
				char *get(void);
				const char *get(void) const;
				file_iterator &operator++(void);
				file_iterator operator++(int);

			private:
				std::shared_ptr<DirData> data;
				std::size_t index = 0;
			};

			class directory_iterator
			{
			private:
				friend class Directory;

				directory_iterator(std::shared_ptr<DirData> data, std::size_t index);

			public:
				typedef const char *DataType;

				directory_iterator(void);
				directory_iterator(const directory_iterator &it);
				directory_iterator(directory_iterator &&it);
				directory_iterator &operator=(const directory_iterator &it);
				directory_iterator &operator=(directory_iterator &&it);
				~directory_iterator(void);

				bool operator==(const directory_iterator &it) const;
				bool operator!=(const directory_iterator &it) const;

				char *operator*(void);
				const char *operator*(void) const;
				char &operator->(void);
				const char &operator->(void) const;
				char *get(void);
				const char *get(void) const;
				directory_iterator &operator++(void);
				directory_iterator operator++(int);

			private:
				std::shared_ptr<DirData> data;
				std::size_t index = 0;
			};

			class iterator
			{
			private:
				friend class Directory;

				iterator(std::shared_ptr<DirData> data, std::size_t index);

			public:
				typedef const char *DataType;

				iterator(void);
				iterator(const iterator &it);
				iterator(iterator &&it);
				iterator &operator=(const iterator &it);
				iterator &operator=(iterator &&it);
				~iterator(void);

				bool operator==(const iterator &it) const;
				bool operator!=(const iterator &it) const;

				char *operator*(void);
				const char *operator*(void) const;
				char &operator->(void);
				const char &operator->(void) const;
				char *get(void);
				const char *get(void) const;
				iterator &operator++(void);
				iterator operator++(int);

			private:
				std::shared_ptr<DirData> data;
				std::size_t index = 0;
			};

			class recursive_iterator
			{
			private:
				friend class Directory;

				recursive_iterator(std::shared_ptr<DirData> data, std::size_t index);

			public:
				typedef const char *DataType;

				recursive_iterator(void);
				recursive_iterator(const recursive_iterator &other);
				recursive_iterator(recursive_iterator &&other);
				recursive_iterator &operator=(const recursive_iterator &it);
				recursive_iterator &operator=(recursive_iterator &&it);
				~recursive_iterator(void);

				bool operator==(const recursive_iterator &it) const;
				bool operator!=(const recursive_iterator &it) const;

				char *operator*(void);
				const char *operator*(void) const;
				char &operator->(void);
				const char &operator->(void) const;
				char *get(void);
				const char *get(void) const;
				recursive_iterator &operator++(void);
				recursive_iterator operator++(int);

			private:
				struct StackData
				{
					std::shared_ptr<DirData> data;
					std::size_t index = 0;
					std::shared_ptr<Directory> dir;

					StackData(void);
					StackData(std::shared_ptr<DirData> data, std::size_t index);
					StackData &operator=(const StackData &other);
					StackData &operator=(StackData &&other);
					~StackData(void);

					bool operator==(const StackData &other) const;
					bool operator!=(const StackData &other) const;
				};

				std::stack<StackData> datas;
			};

			file_iterator file_begin(void) const;
			file_iterator file_end(void) const;
			directory_iterator directory_begin(void) const;
			directory_iterator directory_end(void) const;
			iterator begin(void) const;
			iterator end(void) const;
			recursive_iterator recursive_begin(void) const;
			recursive_iterator recursive_end(void) const;
		};

		void swap(Directory &lhs, Directory &rhs);
	}
}