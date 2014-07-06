#pragma once

#include <Utils/OpenGL.hh>
#include <string>
#include <glm/glm.hpp>
#include <array>


namespace gl
{
	class Shader;
	class UniformBlock;

	//!\file Uniform.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class Uniform
	//!\brief Handle the uniform using in shader
	class ShaderResource
	{
	public:
		ShaderResource();
		ShaderResource(std::string const &flag, Shader const *attach);
		~ShaderResource();
		ShaderResource(ShaderResource const &uniform);
		ShaderResource &operator=(ShaderResource const &uniform);

		template <typename TYPE> TYPE get() const;

		ShaderResource &set(glm::mat4 const &value);
		ShaderResource &set(glm::mat3 const &value);
		ShaderResource &set(int value);
		ShaderResource &set(UniformBlock const &uniformblock);
		ShaderResource &set(float v);
		ShaderResource &set(glm::vec4 const &vec4);

	private:
		std::string _flag;
		void *_data;
		size_t _sizeData;
		Shader const *_attach;
		GLuint _location;

		bool getUniformLocation();
		bool getUniformBlockLocation();
	};

# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	std::cerr << std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "]." << std::endl; return return_type; }


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

}
