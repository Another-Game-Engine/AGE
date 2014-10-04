#include <string>
#include <vector>
#include <initializer_list>
#include "AFormatConvertor.hh"

namespace AGE
{
	AFormatConvertor::AFormatConvertor(std::initializer_list<std::string> formatName)
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