#pragma once

# include <Utils/OpenGL.hh>
# include <iostream>

namespace gl
{
	//!\file Shader.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class Shader
	//!\brief Handle Opengl Shader
	class Shader
	{
	public:
		Shader(std::string const &compute);
		Shader(std::string const &vertex, std::string const &fragment);
		Shader(std::string const &vertex, std::string const &fragment, std::string const &geometry);
		~Shader(void);

		void use();
		bool isValid() const;
		GLuint getId() const;

	private:
		GLuint	_progId;
		GLuint	_vertexId;
		GLuint	_fragId;
		GLuint	_geometryId;
		GLuint	_computeId;

		// tools use inside shader class
		Shader();
		void compileShader(GLuint shaderId, std::string const &file) const;
		void linkProgram() const;
		GLuint addShader(std::string const &path, GLenum type);
		
		// invalid it
		Shader &operator=(Shader const &shader);
		Shader &operator=(Shader &&shader);
		Shader(Shader &&Shader);
		Shader(Shader const &Shader);
	};

}
