#pragma once

#include <string>

namespace gl
{
	class Shader;
	//!\file Uniform.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class Uniform
	//!\brief Handle the uniform using in shader
	class Uniform
	{
	public:
		Uniform();
		Uniform(std::string const &flag, Shader const *attach);
		~Uniform();
		template <typename TYPE> Uniform(std::string const &flag, Shader const *attach, TYPE const &data);
		Uniform(Uniform const &uniform);
		Uniform &operator=(Uniform const &uniform);
		template <typename TYPE> TYPE getData() const;
		template <typename TYPE> Uniform &setData(TYPE const &data);
		template <typename TYPE> Uniform &syncronisation();
	private:
		std::string const _flag;
		void *_data;
		size_t _sizeData;
		Shader const *_attach;
	};

# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	std::cerr << std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "]." << std::endl; return return_type; }

	template <typename TYPE>
	Uniform::Uniform(std::string const &flag, Shader const *attach, TYPE const &data)
		: Uniform(flag)
	{
		_data = new TYPE;
		_sizeData = sizeof(TYPE);
		memcpy(_data, &data, _sizeData);
		_attach = attach;
	}

	template <typename TYPE>
	TYPE Uniform::getData() const
	{
		TYPE value;

		if (sizeof(TYPE) != _sizeData)
			DEBUG_MESSAGE("Error", "Uniform.hh - getData()", "The size of value use for the cast is different of the value intern", value);
		memcpy(&value, _data, _sizeData);
		return (value);
	}

	template <typename TYPE>
	Uniform &Uniform::setData(TYPE const &value)
	{
		if (sizeof(TYPE) != _sizeData)
			DEBUG_MESSAGE("Error", "Uniform.hh - getData()", "The size of value use for the cast is different of the value intern", *this);
		memcpy(_data, &value, _sizeData);
		return (*this);
	}

	// code all syncronisation func you need
}
