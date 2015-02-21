#pragma once

#include "Library.hpp"
#include "Singleton.hpp"

namespace AGE
{
	namespace Engine
	{
		template <class T, class Manager>
		class Plugin;

		template <class T, class Derived>
		class PluginManagerBase : public Singleton<Derived>
		{
			friend class Plugin < T, Derived >;

		public:
			PluginManagerBase(void) = default;
			PluginManagerBase(const PluginManagerBase &other) = delete;
			PluginManagerBase(PluginManagerBase &&other) = delete;
			PluginManagerBase &operator=(const PluginManagerBase &other) = delete;
			PluginManagerBase &operator=(PluginManagerBase &&other) = delete;

			virtual ~PluginManagerBase(void)
			{
				for (auto &p : plugins)
				{
					if (p.plugin != nullptr)
					{
						p.release(p.plugin);
						p.manager.close();
					}
				}
			}

			template <typename... Args>
			bool addPlugin(const std::string &name, Args &&...args)
			{
				AGE_ASSERT(!name.empty() && "Invalid name");
				for (auto &p : plugins)
				{
					if (p.manager.getName() == name)
					{
						AGE_ERROR("\"", name, "\" plugin is already loaded");
						return false;
					}
				}
				plugins.emplace_back(PluginDatas());
				PluginDatas &p = plugins.back();
				if (!p.manager.load(name))
				{
					return false;
				}
				auto create = p.manager.getFunction<T *(Args...)>("CreatePlugin");
				p.release = p.manager.getFunction<void(T *)>("ReleasePlugin");
				if (!create)
				{
					AGE_ERROR("Impossible to find CreatePlugin function in \"", p.manager.getName(), "\" library");
					p.manager.close();
					return false;
				}
				if (!p.release)
				{
					AGE_ERROR("Impossible to find ReleasePlugin function in \"", p.manager.getName(), "\" library");
					p.manager.close();
					return false;
				}
				p.plugin = create(std::forward<Args>(args)...);
				if (p.plugin == nullptr)
				{
					AGE_ERROR("Impossible to create \"", name, "\" plugin");
					p.manager.close();
					return false;
				}
				else
				{
					return onPluginLoaded(name);
				}
			}

		protected:
			virtual bool onPluginLoaded(const std::string &name) = 0;

			struct PluginDatas
			{
				Library manager;
				T *plugin = nullptr;
				std::function<void(T *)> release;

				bool operator==(const PluginDatas &other) const
				{
					return plugin == other.plugin;
				}

				bool operator!=(const PluginDatas &other) const
				{
					return plugin != other.plugin;
				}
			};

		private:
			std::vector<PluginDatas> plugins;
		};

		template <class T>
		class PluginManager final : public PluginManagerBase < T, PluginManager<T> >
		{
			friend Access;

		public:
			virtual ~PluginManager(void) = default;

		private:
			PluginManager(void) = default;
			PluginManager(const PluginManager &other) = delete;
			PluginManager(PluginManager &&other) = delete;
			PluginManager &operator=(const PluginManager &other) = delete;
			PluginManager &operator=(PluginManager &&other) = delete;

			virtual bool onPluginLoaded(const std::string &name) override final
			{
				return true;
			}
		};
	}
}