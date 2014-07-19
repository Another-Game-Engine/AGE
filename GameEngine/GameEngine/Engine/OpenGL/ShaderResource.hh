#pragma once

#include <Utils/OpenGL.hh>
#include <string>
#include <glm/glm.hpp>
#include <array>
#include <cassert>

namespace gl
{
	enum ResourceType
	{
		NONE = 0,
		FLOAT,
		VEC3,
		VEC4,
		MAT3,
		MAT4,
		SAMPLER,
		POINTER_UBO
	};

	//class Shader;
	//class UniformBlock;

	//!\file Uniform.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class Uniform
	//!\brief Handle the uniform using in shader
	class ShaderResource
	{
	public:
		ShaderResource();
		ShaderResource(GLuint location);
		~ShaderResource();
		ShaderResource(ShaderResource const &copy);
		ShaderResource &operator=(ShaderResource const &s);

		template <typename TYPE> TYPE get() const;
		template <typename TYPE> void set(ResourceType setting, TYPE const &value);

	private:
		ResourceType _setting;
		GLuint _location;
		void *_data;
		size_t _sizeData;
	};

# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }


	template <typename TYPE>
	TYPE ShaderResource::get() const
	{
		TYPE value;

		memset(&value, 0, sizeof(TYPE));
		if (sizeof(TYPE) != _sizeData)
			DEBUG_MESSAGE("Error", "Uniform.hh - get()", "The size of type ask is different of data size into this uniform", value);
		memcpy(&value, _data, _sizeData);
		return (value);
	}

	template <typename TYPE>
	void ShaderResource::set(ResourceType setting, TYPE const &value)
	{
		_setting = setting;
		if (sizeof(TYPE) != _sizeData)
		{
			_sizeData = sizeof(TYPE);
			if (_data)
				delete _data;
			_data = new TYPE;
		}
	}
}
