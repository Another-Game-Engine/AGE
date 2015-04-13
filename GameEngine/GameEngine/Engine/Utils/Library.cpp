#include "Library.hpp"

#if !defined(AGE_PLATFORM_WINDOWS)
# include <dlfcn.h>
#endif

namespace AGE
{
	Library::Library(const std::string &name)
	{
		load(name);
	}

	Library::Library(Library &&other)
		: handle(other.handle), pluginName(std::move(other.pluginName))
	{
		other.handle = nullptr;
	}
	
	Library &Library::operator=(Library &&other)
	{
		if (this != &other)
		{
			if (isLoaded())
			{
				close();
			}
			handle = other.handle;
			other.handle = nullptr;
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
#if defined(AGE_PLATFORM_WINDOWS)
			handle = GetModuleHandle(nullptr);
#else
			handle = dlopen(nullptr, 0);
#endif
		}
		else
		{
#if defined(AGE_PLATFORM_WINDOWS)
			handle = LoadLibrary(libraryName.c_str());
#else
			handle = dlopen(libraryName.c_str(), RTLD_LAZY);
#endif
			if (!handle)
			{
				libraryName = name;
#if defined(AGE_PLATFORM_WINDOWS)
				handle = LoadLibrary(libraryName.c_str());
#else
				handle = dlopen(libraryName.c_str(), RTLD_LAZY);
#endif
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
#if defined(AGE_PLATFORM_WINDOWS)
			FreeLibrary(handle);
#else
			dlclose(handle);
#endif
			handle = nullptr;
			pluginName.clear();
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
			static const std::string debugSuffix = "_d";
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