#include <Render/ProgramResources/ABlockResources.hh>

ABlockResources::ABlockResources() :
_offset(0),
_size_array(1),
_stride(0)
{

}

ABlockResources::ABlockResources(ABlockResources const &copy) :
_offset(copy._offset),
_size_array(copy._size_array),
_stride(copy._stride)
{

}

size_t ABlockResources::offset() const
{
	return (_offset);
}

IBlockResources & ABlockResources::offset(size_t o)
{
	_offset = o;
	return (*this);
}

size_t ABlockResources::size_array() const
{
	return (_size_array);
}

IBlockResources & ABlockResources::size_array(size_t sa)
{
	_size_array = sa;
	return (*this);
}

size_t ABlockResources::stride() const
{
	return (_stride);
}

IBlockResources & ABlockResources::stride(size_t s)
{
	_stride = s;
	return (*this);
}

