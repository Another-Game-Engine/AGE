#include <OpenGL/Uniform.hh>
#include <stdint.h>
#include <OpenGL/Shader.hh>

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
		if ((_location = glGetUniformLocation(_attach->getId(), _flag.c_str())) == -1)
			DEBUG_MESSAGE("Error", "Uniform.cpp - getLocation()", "the location [" + _flag + "] doesn't exist on the shader", false)
		return (true);
	}

	// code all syncronisation func you need
	Uniform &Uniform::syncToMat4()
	{
		if (getLocation() == false)
			return (*this);
		if (_sizeData != sizeof(glm::mat4))
			DEBUG_MESSAGE("Error", "Uniform.hh - syncronisation()", "the size of data is not the same of mat4", *this);
		glUniformMatrix4fv(_attach->getId(), 1, GL_FALSE, (float *)_data);
		return (*this);
	}
}