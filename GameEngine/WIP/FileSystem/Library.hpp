#pragma once

#include <string>
#include <functional>
#include <map>
#include <vector>

#include "Architecture.hpp"
#include "Debug.hpp"
#include "Path.hpp"

namespace AGE
{
	namespace Engine
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
			Library(Library &other);
			Library(Library &&other);
			Library &operator=(Library &other);
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
					FARPROC func = GetProcAddress(handle, functionName.c_str());
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
	}
}