#include <string>
#include <vector>
#include "AFormatConvertor.hh"

namespace AGE
{
	AFormatConvertor::AFormatConvertor(const std::vector<std::string> &formatName)
		: _formatHandle(formatName)
	{
	}

	AFormatConvertor::~AFormatConvertor()
	{
	}

	const std::vector<std::string> &AFormatConvertor::getFormatHandle()
	{
		return _formatHandle;
	}
} // namespace AGE