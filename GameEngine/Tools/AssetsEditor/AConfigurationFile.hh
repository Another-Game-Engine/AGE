#pragma once

namespace std
{
	template <class T, class Alloc = allocator<T>> class vector;
	template <class charT, class traits = char_traits<charT>, class Alloc = allocator<charT>> class basic_string;
	typedef basic_string<char> string;
}

namespace AGE
{
	class AConfigurationFile
	{
	protected:
		std::string _fileName;

	public:
		// CTOR
		AConfigurationFile(const std::string &fileName);
		// DTOR
		virtual ~AConfigurationFile();

		// Virtual Pure
		bool readFile();
		bool listDir(const std::string &path, std::vector<std::string> &files);

	}; // class IConfigurationFile
} // namespace AGE