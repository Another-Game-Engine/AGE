#include <vector>
#include <string>
#include <il/devil_cpp_wrapper.hpp>
#include "Texture.hh"

namespace AGE
{
	Texture::Texture()
		: AFormatConvertor({"jpeg", "png"})
	{
	}

	Texture::~Texture()
	{
	}

	bool Texture::load(const std::string &fileName)
	{
		return true;
	}

} // namespace AGE