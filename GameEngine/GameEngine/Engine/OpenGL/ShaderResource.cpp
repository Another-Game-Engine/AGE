#include <OpenGL/ShaderResource.hh>
#include <stdint.h>
#include <OpenGL/Shader.hh>
#include <glm/glm.hpp>
#include <OpenGL/MemoryGPU.hh>

namespace gl
{
	ShaderResource::ShaderResource(std::string const &flag, Shader const *attach)
		: _flag(flag),
		_data(NULL),
		_sizeData(0),
		_attach(attach),
		_location(0)
	{
	}

	ShaderResource::ShaderResource()
		: ShaderResource("", NULL)
	{

	}

	ShaderResource::ShaderResource(ShaderResource const &ressource)
		: ShaderResource(ressource._flag, ressource._attach)
	{
		_sizeData = ressource._sizeData;
		_data = new uint8_t[_sizeData];
		memcpy(_data, ressource._data, _sizeData);
	}

	ShaderResource &ShaderResource::operator=(ShaderResource const &u)
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

	ShaderResource::~ShaderResource()
	{
		if (_data)
			delete _data;
	}

	bool ShaderResource::getUniformLocation()
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
	ShaderResource &ShaderResource::set(glm::mat4 const &value)
	{
		_data = setAllocation<glm::mat4>(_sizeData, _data);
		memcpy(_data, &value, _sizeData);
		if (getUniformLocation() == false)
			return (*this);
		glUniformMatrix4fv(_location, 1, GL_FALSE, &(*(glm::mat4 *)_data)[0].x);
		return (*this);
	}

	ShaderResource &ShaderResource::set(int value)
	{
		_data = setAllocation<glm::mat4>(_sizeData, _data);
		memcpy(_data, &value, _sizeData);
		if (getUniformLocation() == false)
			return (*this);
		glUniform1i(_location, *((int *)value));
		return (*this);
	}

}