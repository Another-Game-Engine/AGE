#pragma once

#include <memory>
#include <mutex>

namespace Age
{
	namespace Engine
	{
		template <class Class>
		class Singleton
		{
		public:
			template <typename... Args>
			static std::shared_ptr<Class> GetInstance(Args &&...args)
			{
				if (!Singleton::Instance)
				{
					std::lock_guard<std::mutex> lock(Singleton::Mutex);
					if (!Singleton::Instance)
					{
						Singleton::Instance.reset(new Class(std::forward<Args>(args)...));
					}
				}
				return Singleton::Instance;
			}

		protected:
			using Access = Singleton<Class>;

			Singleton(void) = default;
			Singleton(const Singleton &) = default;
			Singleton &operator=(const Singleton &) = default;
			virtual ~Singleton(void) = default;

		private:
			static std::mutex Mutex;
			static std::shared_ptr<Class> Instance;
		};

		template <class Class>
		std::mutex Singleton<Class>::Mutex;

		template <class Class>
		std::shared_ptr<Class> Singleton<Class>::Instance;
	}
}