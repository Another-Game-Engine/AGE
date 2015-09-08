#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include "Architecture.hpp"
#include "Debug.hpp"
#include "Path.hpp"

namespace AGE
{
	class Library final
	{
	public:
#if defined(AGE_PLATFORM_WINDOWS)
		using Handle = HMODULE;
#else
		using Handle = void *;
#endif

		Library(void) = default;
		Library(const std::string &name);
		Library(const Library &other) = delete;
		Library(Library &&other);
		Library &operator=(const Library &other) = delete;
		Library &operator=(Library &&other);
		~Library(void);

		bool load(const std::string &name);
		bool isLoaded(void) const;
		void close(void);

		template <typename FunctionSignature>
		std::function<FunctionSignature> getFunction(const std::string &functionName) const
		{
			AGE_ASSERT(handle && "Library is not loaded");
			if (handle)
			{
#if defined(AGE_PLATFORM_WINDOWS)
				FARPROC func = GetProcAddress(handle, functionName.c_str());
#else
				void *func = dlsym(handle, functionName.c_str());
#endif
				if (func != nullptr)
				{
					return std::function<FunctionSignature>(reinterpret_cast<FunctionSignature *>(func));
				}
				else
				{
					AGE_ERROR("Symbol '", functionName, "' not found in library '", pluginName, '\'');
				}
			}
			return std::function<FunctionSignature>();
		}

		const std::string &getName(void) const;
		const Handle &getNativeHandle(void) const;

		static const Library &GetCurrentProcess(void);

	private:
		Handle handle = nullptr;
		std::string pluginName;

		static std::string GenerateName(const std::string &name);
	};

	template <class T>
	class PluginManager
	{
	protected:
		struct PluginData
		{
			using ManagerType = std::shared_ptr < Library > ;
			using ReleaseFunction = std::function < void(T *) > ;

			ManagerType manager = std::make_shared<Library>();
			T *plugin = nullptr;
			ReleaseFunction release;
		};

		using PluginPtr = std::shared_ptr < PluginData > ;
		using PluginContainer = std::unordered_map < std::string, std::pair<std::size_t, PluginPtr> >;

		const char *createFunction;
		const char *releaseFunction;
		static PluginContainer plugins;

	public:
		PluginManager(const char *createFunction = "CreatePlugin", const char *releaseFunction = "ReleasePlugin")
			: createFunction(createFunction), releaseFunction(releaseFunction)
		{
			AGE_ASSERT(createFunction != nullptr && releaseFunction != nullptr && "Invalid function names");
		}

		PluginManager(const PluginManager &other) = delete;
		PluginManager &operator=(const PluginManager &other) = delete;

		virtual ~PluginManager(void)
		{
			PluginContainer::iterator it = plugins.begin();
			while (it != plugins.end())
			{
				if (!--it->second.first)
				{
					it->second.second->release(it->second.second->plugin);
					it->second.second->manager->close();
				}
				else
				{
					++it;
				}
			}
		}

		template <typename... Args>
		bool addPlugin(const std::string &name, Args &&...args)
		{
			AGE_ASSERT(!name.empty() && "Invalid name");
			PluginContainer::iterator found = plugins.find(name);
			if (found != plugins.end())
			{
				++found->second.first;
			}
			PluginPtr p = std::make_shared<PluginData>();
			if (!p->manager->load(name))
			{
				return false;
			}
			auto create = p->manager->getFunction<T *(Args...)>(createFunction);
			p->release = p->manager->getFunction<void(T *)>(releaseFunction);
			if (!create)
			{
				AGE_ERROR("Impossible to find ", createFunction, " function in \"", p->manager->getName(), "\" library");
				p->manager->close();
				return false;
			}
			if (!p->release)
			{
				AGE_ERROR("Impossible to find ", releaseFunction, " function in \"", p->manager->getName(), "\" library");
				p->manager->close();
				return false;
			}
			p->plugin = create(std::forward<Args>(args)...);
			if (p->plugin == nullptr)
			{
				AGE_ERROR("Impossible to create \"", name, "\" plugin");
				p->manager->close();
				return false;
			}
			else
			{
				const bool returnValue = onPluginLoaded(p);
				if (returnValue)
				{
					plugins.insert(std::make_pair(name, std::make_pair(1, p)));
				}
				return returnValue;
			}
		}

		typename PluginData::ManagerType getManager(const std::string &name)
		{
			AGE_ASSERT(!name.empty() && "Invalid name");
			PluginContainer::const_iterator found = plugins.find(name);
			if (found != plugins.end())
			{
				found->second.second->manager;
			}
			else
			{
				return nullptr;
			}
		}

		T *getPlugin(const std::string &name)
		{
			AGE_ASSERT(!name.empty() && "Invalid name");
			PluginContainer::iterator found = plugins.find(name);
			if (found != plugins.end())
			{
				return found->second.second->plugin;
			}
			else
			{
				return nullptr;
			}
		}

		const T *getPlugin(const std::string &name) const
		{
			AGE_ASSERT(!name.empty() && "Invalid name");
			PluginContainer::const_iterator found = plugins.find(name);
			if (found != plugins.end())
			{
				found->second.second->plugin;
			}
			else
			{
				return nullptr;
			}
		}

		virtual bool onPluginLoaded(PluginPtr pluginData)
		{
			return true;
		}
	};

	template <class T>
	typename PluginManager<T>::PluginContainer PluginManager<T>::plugins;
}