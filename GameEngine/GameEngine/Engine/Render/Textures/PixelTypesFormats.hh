#pragma once

# include <Utils/OpenGL.hh>
# include <unordered_map>

struct PixelType
{
public:
	size_t size;

public:
	PixelType() :
		size(0)
	{
	}
	PixelType(size_t size) :
		size(size)
	{
	}
	PixelType(PixelType const &copy) :
		size(copy.size)
	{

	}
	PixelType(PixelType &&move) :
		size(move.size)
	{

	}
};

std::unordered_map<GLenum, PixelType> available_type_pixel =
{
	{ GL_UNSIGNED_BYTE, PixelType(1) },
	{ GL_UNSIGNED_INT, PixelType(4) },
	{ GL_UNSIGNED_SHORT, PixelType(2) },
	{ GL_UNSIGNED_INT_24_8, PixelType(4) },
	{ GL_FLOAT, PixelType(4) }
};