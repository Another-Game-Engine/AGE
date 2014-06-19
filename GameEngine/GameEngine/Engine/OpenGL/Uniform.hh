#pragma once

#include <Utils/OpenGL.hh>
#include <string>
#include <glm/glm.hpp>

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
		Uniform(Uniform const &uniform);
		Uniform &operator=(Uniform const &uniform);

		template <typename TYPE> TYPE get() const;

		Uniform &set(glm::mat4 const &value);
		Uniform &set(int value);

	private:
		std::string _flag;
		void *_data;
		size_t _sizeData;
		Shader const *_attach;
		GLuint _location;

		bool getLocation();
	};

# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	std::cerr << std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "]." << std::endl; return return_type; }


	template <typename TYPE>
	TYPE Uniform::get() const
	{
		TYPE value;

		memset(&value, 0, sizeof(TYPE));
		if (sizeof(TYPE) != _sizeData)
			DEBUG_MESSAGE("Error", "Uniform.hh - get()", "The size of type ask is different of data size into this uniform", value);
		memcpy(&value, _data, _sizeData);
		return (value);
	}
}
