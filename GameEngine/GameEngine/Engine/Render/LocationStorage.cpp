#include <Render/LocationStorage.hh>
#include <stdint.h>
#include <glm/glm.hpp>

namespace gl
{
	Location::Location()
		: ptr(NULL),
		size(0),
		nbrElement(0)
	{

	}

	Location::Location(void const *location, size_t size, size_t nbrElement)
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

	LocationStorage::LocationStorage()
	{
	}

	LocationStorage::~LocationStorage()
	{
	}

	LocationStorage &LocationStorage::generateLocation(size_t nbrIndex)
	{
		if (nbrIndex == _locations.size())
			return (*this);
		_locations.resize(nbrIndex);
		return (*this);
	}
}