#include <OpenGL/Uniform.hh>
#include <stdint.h>
#include <OpenGL/Shader.hh>

namespace gl
{
	Uniform::Uniform(std::string const &flag, Shader const *attach)
		: _flag(flag),
		_data(NULL),
		_sizeData(0),
		_attach(attach)
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
}