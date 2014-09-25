#pragma once

#include "AConfigurationFile.hh"
#include "dataSet.hh"

namespace std
{
	template <class charT, class traits = char_traits<charT>, class Alloc = allocator<charT>> class basic_string;
	typedef basic_string<char> string;
}

namespace AGE
{
	class Simple: public AConfigurationFile
	{
	private:
		const std::string _fileName;
	public:
		// CTOR
		explicit Simple(const std::string &fileName);
		// DTOR
		virtual ~Simple();

	private:
		// Private members functions
		bool _readFile();

	}; // class Simple
} // namespace AGE