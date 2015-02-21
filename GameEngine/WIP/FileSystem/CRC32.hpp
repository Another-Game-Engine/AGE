#pragma once

#include <cstddef>

namespace AGE
{
	namespace Engine
	{
		class CRC32 final
		{
		public:
			CRC32(void) = default;
			CRC32(const CRC32 &other) = default;
			CRC32 &operator=(const CRC32 &other) = default;
			~CRC32(void) = default;

			void begin(void);
			void update(const void *data, std::size_t size);
			void end(void);
			std::size_t get(void) const;

			static std::size_t Calculate(const void *data, std::size_t size);

		private:
			std::size_t value = 0;
		};
	}
}