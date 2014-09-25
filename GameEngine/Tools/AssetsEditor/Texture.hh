#pragma once

#include "AFormatConvertor.hh"

namespace AGE
{
	class Texture : public AFormatConvertor
	{
	private:

	public:
		// CTOR
		explicit Texture();
		// DTOR
		virtual ~Texture();

		bool load(const std::string &fileName);
	}; // class Texture
} // namespace AGE