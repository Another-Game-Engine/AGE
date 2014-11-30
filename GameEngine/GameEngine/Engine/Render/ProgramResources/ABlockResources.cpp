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

/**
* Method:    offset
* FullName:  ABlockResources::offset
* Access:    virtual public 
* Returns:   size_t
* Qualifier: const
* Goal:		 getter on offset
*/
size_t ABlockResources::offset() const
{
	return (_offset);
}

/**
* Method:    offset
* FullName:  ABlockResources::offset
* Access:    virtual public 
* Returns:   IBlockResources &
* Qualifier:
* Parameter: size_t o
* Goal:		 setter for offset
*/
IBlockResources & ABlockResources::offset(size_t o)
{
	_offset = o;
	return (*this);
}

/**
* Method:    size_array
* FullName:  ABlockResources::size_array
* Access:    virtual public 
* Returns:   size_t
* Qualifier: const
* Goal:		 size of the array
*/
size_t ABlockResources::size_array() const
{
	return (_size_array);
}

/**
* Method:    size_array
* FullName:  ABlockResources::size_array
* Access:    virtual public 
* Returns:   IBlockResources &
* Qualifier:
* Parameter: size_t sa
* Goal:		 set the size of the array
*/
IBlockResources & ABlockResources::size_array(size_t sa)
{
	_size_array = sa;
	return (*this);
}

/**
* Method:    stride
* FullName:  ABlockResources::stride
* Access:    virtual public 
* Returns:   size_t
* Qualifier: const
* Goal:		 get the stride
*/
size_t ABlockResources::stride() const
{
	return (_stride);
}

/**
* Method:    stride
* FullName:  ABlockResources::stride
* Access:    virtual public 
* Returns:   IBlockResources &
* Qualifier:
* Parameter: size_t s
* Goal:		 set the stride
*/
IBlockResources & ABlockResources::stride(size_t s)
{
	_stride = s;
	return (*this);
}

