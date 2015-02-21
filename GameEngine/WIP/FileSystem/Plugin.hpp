#pragma once

#include "PluginManager.hpp"

namespace AGE
{
	namespace Engine
	{
		template <class T, class Manager = PluginManager<T>>
		class Plugin final
		{
			friend Manager;

		public:
			template <typename... Args>
			static T *Load(const std::string &name, Args &&...args)
			{
				std::shared_ptr<PluginManagerBase<T, Manager>> manager = std::static_pointer_cast<PluginManagerBase<T, Manager>>(Manager::GetInstance());
				if (manager->addPlugin(name, std::forward<Args>(args)...))
				{
					for (auto &p : manager->plugins)
					{
						if (p.manager.getName() == name)
						{
							return p.plugin;
						}
					}
				}
				return nullptr;
			}

			static void Unload(const std::string &name)
			{
				AGE_ASSERT(!name.empty() && "Invalid name");
				std::shared_ptr<PluginManagerBase<T, Manager>> manager = std::static_pointer_cast<PluginManagerBase<T, Manager>>(Manager::GetInstance());
				for (auto &p : manager->plugins)
				{
					if (p.plugin && p.manager.getName() == name)
					{
						p.release(p.plugin);
						manager->plugins.erase(std::find(manager->plugins.begin(), manager->plugins.end(), p));
						break;
					}
				}
			}

			static void Unload(T *plugin)
			{
				AGE_ASSERT(plugin != nullptr && "Invalid plugin");
				std::shared_ptr<PluginManagerBase<T, Manager>> manager = std::static_pointer_cast<PluginManagerBase<T, Manager>>(Manager::GetInstance());
				for (auto &p : manager->plugins)
				{
					if (p.plugin != nullptr && p.plugin == plugin)
					{
						p.release(plugin);
						manager->plugins.erase(std::find(manager->plugins.begin(), manager->plugins.end(), p));
						break;
					}
				}
			}

		private:
			Plugin(void) = delete;
			Plugin(const Plugin &other) = delete;
			Plugin(Plugin &&other) = delete;
			Plugin &operator=(const Plugin &other) = delete;
			Plugin &operator=(Plugin &&other) = delete;
			~Plugin(void) = delete;
		};
	}
}