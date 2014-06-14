#pragma once

# include <Utils/OpenGL.hh>
# include <iostream>
# include <map>
# include <OpenGL/Key.hh>

namespace gl
{
	class Uniform;

	//!\file Shader.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class Shader
	//!\brief Handle Opengl Shader
	class Shader
	{
	public:
		Shader();
		Shader(std::string const &compute);
		Shader(std::string const &vertex, std::string const &fragment);
		Shader(std::string const &vertex, std::string const &fragment, std::string const &geometry);
		Shader(Shader const &Shader);
		Shader &operator=(Shader const &shader);
		~Shader(void);

		void use();
		bool isValid() const;
		GLuint getId() const;

		std::string const &getVertexName() const;
		std::string const &getFragName() const;
		std::string const &getGeoName() const;
		std::string const &getComputeName() const;

		// handling uniform management
		Key<Uniform> addUniform(std::string const &flag);
		template <typename TYPE> Key<Uniform> addUniform(std::string const &flag, TYPE const &value);
		Shader &rmUniform(Key<Uniform> &key);
		Key<Uniform> getUniform(size_t index) const;
		template <typename TYPE> Shader &setUniform(Key<Uniform> const &key, TYPE const &value);

	private:
		std::string _vertexName;
		std::string _fragName;
		std::string _geometryName;
		std::string _computeName;
		GLuint	_progId;
		GLuint	_vertexId;
		GLuint	_fragId;
		GLuint	_geometryId;
		GLuint	_computeId;

		std::map<Key<Uniform>, Uniform> _uniforms;

		// some private function usefull for internal functionement
		bool compileShader(GLuint shaderId, std::string const &file) const;
		bool linkProgram() const;
		GLuint addShader(std::string const &path, GLenum type);
		
	};

# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	std::cerr << std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "]." << std::endl; return return_type; }


	template <typename TYPE>
	Key<Uniform> Shader::addUniform(std::string const &flag, TYPE const &value)
	{
		Key<Uniform> key;

		_uniform[key] = Uniform(flag, this, value);
		return (key);
	}

	template <typename TYPE>
	Shader &Shader::setUniform(Key<Uniform> const &key, TYPE const &value)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.hh - setUniform(key, value)", "key destroy use", *this);
		auto &element = _uniforms.find(key);
		if (element == _uniforms.end())
			DEBUG_MESSAGE("Warning", "Shader.hh - setUniform(key, value)", "element in not find", *this);
		element->second.setData(value);
		return (*this);
	}
}
