#include <string>
#include <vector>

#include "AssetConvertor.hh"
#include "AConfigurationFile.hh"
#include "AFormatConvertor.hh"

namespace AGE
{
	AssetConvertor::AssetConvertor(AConfigurationFile *file)
		: _confFile(file)
	{
	}

	AssetConvertor::~AssetConvertor()
	{
	}

	int AssetConvertor::loads()
	{
		int ret = 0;

		return ret;
	}

	int AssetConvertor::saves()
	{
		int ret = 0;

		return ret;
	}

} // namespace AGE