#include <string>
#include <iostream>
#include "SimpleConf.hh"

namespace AGE
{
	SimpleConf::SimpleConf(const std::string &fileName)
		: AConfigurationFile(fileName)
		, _fileName(fileName)
	{
		if (!_readFile())
		{
			std::cerr << "Error while reading " << fileName << std::endl;
		}
	}

	SimpleConf::~SimpleConf()
	{
	}

	bool SimpleConf::_readFile()
	{
		return true;
	}

} // namespace AGE