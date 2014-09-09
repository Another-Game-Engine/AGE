#include <Render/ShaderResourceManager.hh>
#include <stdint.h>

namespace gl
{
	Location::Location()
		: ptr(NULL),
		size(0),
		nbrElement(0)
	{

	}

	Location::Location(void *location, size_t size, size_t nbrElement)
		: ptr(NULL),
		size(size),
		nbrElement(nbrElement)
	{
		this->ptr = new uint8_t[size];
		memcpy(this->ptr, location, size);
	}

	Location::~Location()
	{
		if (ptr != NULL)
			delete[] ptr;
	}

	Location::Location(Location const &copy)
		: ptr(NULL),
		size(copy.size),
		nbrElement(copy.nbrElement)
	{
		ptr = new uint8_t[size];
		memcpy(ptr, copy.ptr, size);
	}

	Location &Location::operator=(Location const &copy)
	{
		if (ptr != NULL)
			delete[] ptr;
		size = copy.size;
		nbrElement = copy.nbrElement;
		ptr = new uint8_t[size];
		memcpy(ptr, copy.ptr, size);
		return (*this);
	}

	ShaderResourceManager::ShaderResourceManager()
	{
	}

	ShaderResourceManager::~ShaderResourceManager()
	{
	}

	ShaderResourceManager &ShaderResourceManager::generateLocation(size_t nbrIndex)
	{
		_locations.resize(nbrIndex);
		return (*this);
	}
}