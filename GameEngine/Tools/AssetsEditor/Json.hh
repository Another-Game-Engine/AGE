#pragma once

#include "IConfigurationFile.hh"

namespace std
{
	template <class charT, class traits = char_traits<charT>, class Alloc = allocator<charT>> class basic_string;
	typedef basic_string<char> string;
}

namespace AGE
{
	class Json: public IConfigurationFile
	{
	private:

	public:
		// CTOR
		explicit Json(const std::string &fileName);
		// DTOR
		virtual ~Json();
	}; // class Json
} // namespace AGE