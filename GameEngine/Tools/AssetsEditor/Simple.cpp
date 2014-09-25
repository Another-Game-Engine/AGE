#include <string>
#include <iostream>
#include "Simple.hh"

namespace AGE
{
	Simple::Simple(const std::string &fileName)
		: _fileName(fileName)
	{
		if (!_readFile())
		{
			std::cerr << "Error while reading " << fileName << std::endl;
		}
	}

	Simple::~Simple()
	{
	}

	bool Simple::_readFile()
	{
		return true;
	}

} // namespace AGE