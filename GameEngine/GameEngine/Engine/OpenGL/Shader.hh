#pragma once

# include <Utils/OpenGL.hh>
# include <iostream>
# include <map>
# include <OpenGL/Key.hh>
# include <glm/glm.hpp>

namespace gl
{
	class Uniform;
	class Texture;

	struct Sampler{};

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

		void use() const;
		bool isValid() const;
		GLuint getId() const;

		std::string const &getVertexName() const;
		std::string const &getFragName() const;
		std::string const &getGeoName() const;
		std::string const &getComputeName() const;

		// handling uniform management
		Key<Uniform> addUniform(std::string const &flag);
		Key<Uniform> addUniform(std::string const &flag, glm::mat4 const &value);
		Shader &rmUniform(Key<Uniform> &key);
		Key<Uniform> getUniform(size_t index) const;
		Shader &setUniform(Key<Uniform> const &key, glm::mat4 const &value);
		// sampler 
		Key<Sampler> addSampler(std::string const &flag);
		Shader &rmSampler(Key<Sampler> key);
		Key<Sampler> getSampler(size_t index) const;
		Shader &setSampler(Key<Sampler> const &key, Texture const &bind);

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

		std::map<Key<Sampler>, Uniform> _samplers;
		bool _units[GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS];

		// some private function usefull for internal functionement
		bool compileShader(GLuint shaderId, std::string const &file) const;
		bool linkProgram() const;
		GLuint addShader(std::string const &path, GLenum type);
		
	};
}
