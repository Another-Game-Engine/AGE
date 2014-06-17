#include <OpenGL/Uniform.hh>
#include <stdint.h>
#include <OpenGL/Shader.hh>
#include <glm/glm.hpp>

# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	std::cerr << std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "]." << std::endl; return return_type; }

namespace gl
{
	Uniform::Uniform(std::string const &flag, Shader const *attach)
		: _flag(flag),
		_data(NULL),
		_sizeData(0),
		_attach(attach),
		_location(0)
	{
	}

	Uniform::Uniform()
		: Uniform("", NULL)
	{

	}

	Uniform::Uniform(Uniform const &uniform)
		: Uniform(uniform._flag, uniform._attach)
	{
		_sizeData = uniform._sizeData;
		_data = new uint8_t[_sizeData];
		memcpy(_data, uniform._data, _sizeData);
	}

	Uniform &Uniform::operator=(Uniform const &u)
	{
		if (this != &u)
		{
			_location = u._location;
			if (u._sizeData != _sizeData)
			{
				_sizeData = u._sizeData;
				if (_data)
					delete _data;
				_data = new uint8_t[_sizeData];
			}
			memcpy(_data, u._data, _sizeData);
			_attach = u._attach;
		}
		return (*this);
	}

	Uniform::~Uniform()
	{
		if (_data)
			delete _data;
	}

	bool Uniform::getLocation()
	{
		if (_attach == NULL)
			DEBUG_MESSAGE("Error", "Uniform.cpp - getLocation()", "No attach on this uniform", false);
		_attach->use();
		if ((_location = glGetUniformLocation(_attach->getId(), _flag.c_str())) == -1)
			DEBUG_MESSAGE("Error", "Uniform.cpp - getLocation()", "the location [" + _flag + "] doesn't exist on the shader", false)
		return (true);
	}

	template <typename TYPE>
	static void *setAllocation(size_t sizeData, void *data)
	{
		if (sizeof(TYPE) != sizeData)
		{
			sizeData = sizeof(TYPE);
			if (data)
				delete data;
			data = new TYPE;
		}
		return (data);
	}

	// code all set syncronisation func you need
	Uniform &Uniform::set(glm::mat4 const &value)
	{
		_data = setAllocation<glm::mat4>(_sizeData, _data);
		memcpy(_data, &value, _sizeData);
		if (getLocation() == false)
			return (*this);
		if (_sizeData != sizeof(glm::mat4))
			DEBUG_MESSAGE("Error", "Uniform.hh - syncronisation()", "the size of data is not the same of mat4", *this);
		glUniformMatrix4fv(_attach->getId(), 1, GL_FALSE, (float *)_data);
		return (*this);
	}
}