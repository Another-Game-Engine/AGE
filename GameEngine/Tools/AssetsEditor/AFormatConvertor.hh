#pragma once

#include <string>

namespace std
{
	template <class T, class Alloc = allocator<T>> class vector;
}

namespace AGE
{
	class AFormatConvertor
	{
	private:
		std::vector<std::string> _formatHandle;
	public:
		// CTOR
		AFormatConvertor(const std::vector<std::string> &formatName);
		// DTOR
		virtual ~AFormatConvertor();
		// GET
		const std::vector<std::string> &getFormatHandle();

	    // Pure Virtual

	}; // class AFormatConvertor
} // namespace AGE