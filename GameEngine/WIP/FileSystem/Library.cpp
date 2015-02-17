#include "Library.hpp"

namespace Age
{
	namespace Engine
	{
		Library::Library(const std::string &name)
		{
			load(name);
		}

		Library::Library(Library &other)
			: handle(other.handle), pluginName(other.pluginName)
		{
			other.handle = nullptr;
			other.pluginName.clear();
		}

		Library::Library(Library &&other)
			: handle(std::move(other.handle)), pluginName(std::move(other.pluginName))
		{
			return;
		}

		Library &Library::operator=(Library &other)
		{
			if (this != &other)
			{
				if (isLoaded())
				{
					close();
				}
				handle = other.handle;
				pluginName = other.pluginName;
				other.handle = nullptr;
				other.pluginName.clear();
			}
			return *this;
		}

		Library &Library::operator=(Library &&other)
		{
			if (this != &other)
			{
				if (isLoaded())
				{
					close();
				}
				handle = std::move(other.handle);
				pluginName = std::move(other.pluginName);
			}
			return *this;
		}

		Library::~Library(void)
		{
			if (isLoaded())
			{
				close();
			}
		}

		bool Library::load(const std::string &name)
		{
			AGE_ASSERT(handle == nullptr && "Library is not closed");
			std::string libraryName = Library::GenerateName(name);
			if (libraryName.empty())
			{
				handle = GetModuleHandle(nullptr);
			}
			else
			{
				handle = LoadLibrary(libraryName.c_str());
				if (!handle)
				{
					libraryName = name;
					handle = LoadLibrary(libraryName.c_str());
				}
			}
			if (handle != nullptr)
			{
				if (libraryName.empty())
				{
					pluginName = "CurrentProcess";
				}
				else
				{
					pluginName = libraryName;
				}
				return true;
			}
			else
			{
				AGE_ERROR("Impossible to load \"", libraryName, "\" library");
				return false;
			}
		}

		bool Library::isLoaded(void) const
		{
			return handle != nullptr;
		}

		void Library::close(void)
		{
			AGE_ASSERT(handle && "Library not opened");
			if (handle)
			{
				FreeLibrary(handle);
			}
		}

		const std::string &Library::getName(void) const
		{
			return pluginName;
		}

		const Library::Handle &Library::getNativeHandle(void) const
		{
			return handle;
		}

		const Library &Library::GetCurrentProcess(void)
		{
			static Library currentProcess;
			if (!currentProcess.isLoaded())
			{
				currentProcess.load("");
			}
			return currentProcess;
		}

		std::string Library::GenerateName(const std::string &name)
		{
			if (name.empty())
			{
				return std::string();
			}
			else
			{
#if defined(AGE_32_BIT)
				static const std::string architectureSuffix = "_x86";
#else
				static const std::string architectureSuffix = "_x64";
#endif
#if defined(AGE_DEBUG)
				static const std::string debugSuffix = "d";
#else
				static const std::string debugSuffix = "";
#endif
#if defined(AGE_PLATFORM_WINDOWS)
				static const std::string prefix = "";
				static const std::string extension = ".dll";
#elif defined(AGE_PLATFORM_LINUX) || defined(AGE_PLATFORM_ANDROID)
				static const std::string prefix = "lib";
				static const std::string extension = ".so";
#elif defined(AGE_PLATFORM_MACOS) || defined(AGE_PLATFORM_IOS)
				static const std::string prefix = "lib";
				static const std::string extension = ".dylib";
#endif
				const std::string libraryPath = Path::PathName(name.c_str());
				return libraryPath + prefix + Path::BaseName(name.c_str()) + architectureSuffix + debugSuffix + extension;
			}
		}
	}
}
