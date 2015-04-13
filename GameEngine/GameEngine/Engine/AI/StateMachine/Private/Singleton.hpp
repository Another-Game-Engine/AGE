#pragma once

#include <memory>
#include <mutex>

namespace AGE
{
	namespace AI
	{
		namespace Private
		{
			template <class Class>
			class Singleton
			{
			public:
				// Static Methods
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
				using Access = Singleton < Class > ;

				// Constructors
				Singleton(void) = default;

				Singleton(Singleton const &) = default;

				// Assignment operators
				Singleton &operator=(Singleton const &) = default;

				// Destructor
				virtual ~Singleton(void) = default;

			private:
				// Static attributes
				static std::mutex Mutex;

				static std::shared_ptr<Class> Instance;
			};

			// Static attributes
			template <class Class>
			std::mutex Singleton<Class>::Mutex;

			template <class Class>
			std::shared_ptr<Class> Singleton<Class>::Instance;
		}
	}
}
