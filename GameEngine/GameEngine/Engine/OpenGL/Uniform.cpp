#include <OpenGL/Uniform.hh>
#include <stdint.h>
#include <OpenGL/Shader.hh>
#include <glm/glm.hpp>
#include <OpenGL/MemoryBlocksGPU.hh>

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
			_flag = u._flag;
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
	static void *setAllocation(size_t &sizeData, void *data)
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
		glUniformMatrix4fv(_location, 1, GL_FALSE, &(*(glm::mat4 *)_data)[0].x);
		return (*this);
	}

	Uniform &Uniform::set(int value)
	{
		_data = setAllocation<glm::mat4>(_sizeData, _data);
		memcpy(_data, &value, _sizeData);
		if (getLocation() == false)
			return (*this);
		glUniform1i(_location, *((int *)value));
		return (*this);
	}

	UniformBlock::UniformBlock()
		: _elements(NULL),
		_nbrElement(0),
		_attach(NULL),
		_location(0)
	{
	}

	UniformBlock::~UniformBlock()
	{
		clean();
	}

	UniformBlock::UniformBlock(UniformBlock const &copy)
		: UniformBlock()
	{
		_location = copy._location;
		_flag = copy._flag;
		_attach = copy._attach;
		_nbrElement = copy._nbrElement;
		if (_nbrElement != 0)
		{
			_elements = new std::string[_nbrElement];
			for (size_t index = 0; index < _nbrElement; ++index)
				_elements[index] = copy._elements[index];
		}
	}

	UniformBlock::UniformBlock(std::string const &flag, Shader const *attach)
		: UniformBlock()
	{
		_flag = flag;
		_attach = attach;
	}

	UniformBlock &UniformBlock::operator=(UniformBlock const &u)
	{
		if (this == &u)
		{
			_location = u._location;
			_flag = u._flag;
			_attach = u._attach;
			if (_nbrElement != u._nbrElement)
			{
				_nbrElement = u._nbrElement;
				clean();
				if (_nbrElement != 0)
				{
					_elements = new std::string[_nbrElement];
				}
				else
				{
					_elements = NULL;
				}
			}
			for (size_t index = 0; index < _nbrElement; ++index)
				_elements[index] = u._elements[index];
		}
		return (*this);
	}

	void UniformBlock::set(size_t nbrElement, std::string const *elements, size_t const *sizeData)
	{
		if (nbrElement != _nbrElement)
		{
			_nbrElement = nbrElement;
			clean();
			if (_nbrElement != 0)
			{
				_elements = new std::string[_nbrElement];
			}
			else
			{
				_elements = NULL;
				return;
			}
		}
		for (size_t index = 0; index < _nbrElement; ++index)
		{
			size_t cumulOffset = 0;
			_elements[index] = elements[index];
		}
	}

	void UniformBlock::clean()
	{
		if (_elements)
			delete _elements;
	}
}