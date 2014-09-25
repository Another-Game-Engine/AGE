#pragma once

namespace std
{
	template <class T, class Alloc = allocator<T>> class vector;
	template<class T> class initializer_list;
}

namespace AGE
{
	class AFormatConvertor
	{
	private:
		const std::vector<std::string> _formatHandle;
	protected:
	public:
		// CTOR
		explicit AFormatConvertor(std::initializer_list<std::string> formatName);
		// DTOR
		virtual ~AFormatConvertor();
		// GET
		const std::vector<std::string> &getFormatHandle();
	    // Pure Virtual
		virtual bool load(const std::string &fileName) = 0;
		virtual bool save() = 0;

	}; // class AFormatConvertor
} // namespace AGE