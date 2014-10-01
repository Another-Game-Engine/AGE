#pragma once

namespace std
{
	template <class charT, class traits = char_traits<charT>, class Alloc = allocator<charT>> class basic_string;
	typedef basic_string<char> string;
}

namespace AGE
{
	class dataSet
	{
	private:
		
	public:
		// CTOR
		explicit dataSet();
		// DTOR
		virtual ~dataSet();


	}; // class dataSet
} // namespace AGE