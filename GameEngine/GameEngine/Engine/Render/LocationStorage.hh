#pragma once

#include <Utils/Containers/Vector.hpp>
#include <assert.h>
#include <cstring>

namespace gl
{
	struct Location
	{
		void *ptr;
		size_t size;
		size_t nbrElement;
		Location();
		Location(void const *location, size_t size, size_t nbrElement);
		~Location();
		Location(Location const &copy);
		Location &operator=(Location const &l);
	};

	class LocationStorage
	{
	public:
		LocationStorage();
		~LocationStorage();

		LocationStorage &generateLocation(size_t nbrIndex);
		template <typename TYPE> LocationStorage &setLocation(size_t index, TYPE const &value);
		template <typename TYPE> LocationStorage &setLocation(size_t index, TYPE const &value, size_t nbrElement);
		template <typename TYPE> TYPE getLocation(size_t index);
		template <typename TYPE> TYPE getLocation(size_t index, size_t &nbrElement);
		

	private:
		LocationStorage(LocationStorage const &copy) = delete;
		LocationStorage &operator=(LocationStorage const &s);

		AGE::Vector<Location> _locations;
	};

	template <typename TYPE>
	LocationStorage &LocationStorage::setLocation(size_t index, TYPE const &value)
	{
		if (_locations[index].size != sizeof(TYPE))
			_locations[index] = Location(&value, sizeof(TYPE), 1);
		else
		{
			memcpy(_locations[index].ptr, (TYPE *)&value, sizeof(TYPE));
			_locations[index].nbrElement = 1;
		}
		return (*this);
	}

	template <typename TYPE>
	LocationStorage &LocationStorage::setLocation(size_t index, TYPE const &value, size_t nbrElement)
	{
		if (_locations[index].size != sizeof(TYPE))
			_locations[index] = Location(&value, sizeof(TYPE), nbrElement);
		else
		{
			memcpy(_locations[index].ptr, (TYPE *)&value, sizeof(TYPE));
			_locations[index].nbrElement = nbrElement;
		}
		return (*this);
	}

	template <typename TYPE>
	TYPE LocationStorage::getLocation(size_t index)
	{
		if (sizeof(TYPE) != _locations[index].size)
			assert(0);
		Location &location = _locations[index];
		TYPE value;
		memcpy(&value, location.ptr, sizeof(TYPE));
		return (value);
	}

	template <typename TYPE>
	TYPE LocationStorage::getLocation(size_t index, size_t &nbrElement)
	{
		nbrElement = _locations[index].nbrElement;
		return (getLocation(index));
	}
}