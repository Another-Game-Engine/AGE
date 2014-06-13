#include <OpenGL/Uniform.hh>
#include <stdint.h>

namespace gl
{
	Uniform::Uniform(std::string const &flag)
		: _flag(flag),
		_data(NULL),
		_sizeData(0)
	{

	}

	Uniform::Uniform()
		: Uniform("")
	{

	}

	Uniform::Uniform(Uniform const &uniform)
		: Uniform(uniform._flag)
	{
		_sizeData = uniform._sizeData;
		_data = new uint8_t[_sizeData];
		memcpy(_data, uniform._data, _sizeData);
	}

	Uniform &Uniform::operator=(Uniform const &u)
	{
		if (this != &u)
		{
			if (u._sizeData != _sizeData)
			{
				_sizeData = u._sizeData;
				if (_data)
					delete _data;
				_data = new uint8_t[_sizeData];
			}
			memcpy(_data, u._data, _sizeData);
		}
		return (*this);
	}

	Uniform::~Uniform()
	{
		if (_data)
			delete _data;
	}
}