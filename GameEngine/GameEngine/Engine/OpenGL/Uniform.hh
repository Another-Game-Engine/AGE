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

}
