#ifndef SHADER_HH_
# define SHADER_HH_

# include <Utils/OpenGL.hh>

namespace OpenGLTools
{

	class Shader
	{
	public:
		Shader(std::string const &compute);
		Shader(std::string const &vertex, std::string const &fragment);
		Shader(std::string const &vertex, std::string const &fragment, std::string const &geometry);
		~Shader(void);
		Shader(Shader &&Shader);
		Shader(Shader const &Shader);

		void use();
		bool isValid() const;
		GLuint	getId() const;

	private:
		GLuint	_progId;
		GLuint	_vertexId;
		GLuint	_fragId;
		GLuint	_geometryId;
		GLuint	_computeId;

		Shader();
		void compileShader(GLuint shaderId, std::string const &file) const;
		void linkProgram() const;
		GLuint addShader(std::string const &path, GLenum type);
		Shader &operator=(Shader const &shader);
		Shader &operator=(Shader &&shader);

	};

}

#endif /*!SHADER_HH_*/
