#pragma once

#include <Utils/Containers/Vector.hpp>
#include <assert.h>

namespace gl
{
	struct Location
	{
		void *ptr;
		size_t size;
		size_t nbrElement;
		Location();
		Location(void *location, size_t size, size_t nbrElement);
		~Location();
		Location(Location const &copy);
		Location &operator=(Location const &l);
	};

	class ShaderResourceManager
	{
	public:
		ShaderResourceManager();
		~ShaderResourceManager();

		ShaderResourceManager &generateLocation(size_t nbrIndex);
		template <typename TYPE> ShaderResourceManager &setLocation(size_t index, TYPE const &value);
		template <typename TYPE> ShaderResourceManager &setLocation(size_t index, TYPE const &value, size_t nbrElement);
		template <typename TYPE> TYPE getLocation(size_t index);
		template <typename TYPE> TYPE getLocation(size_t index, size_t &nbrElement);

	private:
		ShaderResourceManager(ShaderResourceManager const &copy) = delete;
		ShaderResourceManager &operator=(ShaderResourceManager const &s);

		AGE::Vector<Location> _locations;
	};

	template <typename TYPE>
	ShaderResourceManager &ShaderResourceManager::setLocation(size_t index, TYPE const &value)
	{
		_locations[index] = Location(value, sizeof(TYPE), 1);
		return (*this);
	}

	template <typename TYPE>
	ShaderResourceManager &ShaderResourceManager::setLocation(size_t index, TYPE const &value, size_t nbrElement)
	{
		_locations[index] = Location(value, sizeof(TYPE), nbrElement);
		return (*this);
	}

	template <typename TYPE>
	TYPE ShaderResourceManager::getLocation(size_t index)
	{
		if (sizeof(TYPE) != _locations[index].size)
			assert(0);
		Location &location = _locations[index];
		TYPE value;
		memcpy(&value, location.ptr, sizeof(TYPE)* location.size());
		return (value);
	}

	template <typename TYPE>
	TYPE ShaderResourceManager::getLocation(size_t index, size_t &nbrElement)
	{
		nbrElement = _locations[index].nbrElement;
		return (getLocation(index));
	}
}