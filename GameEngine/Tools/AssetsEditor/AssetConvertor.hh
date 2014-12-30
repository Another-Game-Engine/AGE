#pragma once

namespace std
{
	template <class T, class Alloc = allocator<T>> class vector;
	template <class charT, class traits = char_traits<charT>, class Alloc = allocator<charT>> class basic_string;
	typedef basic_string<char> string;
}

namespace AGE
{
	class AConfigurationFile;
	class AFormatConvertor;
	class dataSet;
} // namespace AGE

namespace AGE
{
	class AssetConvertor
	{
	private:
		AConfigurationFile *_confFile;
		std::vector<AFormatConvertor*> _formatFiles;
	public:
		// CTOR
		AssetConvertor(AConfigurationFile *file);
		// DTOR
		~AssetConvertor();

		//
		int loads();
		int saves();
	}; // class AssetConvertor
} // namespace AGE