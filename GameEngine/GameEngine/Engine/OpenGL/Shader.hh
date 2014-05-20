#ifndef SHADER_HH_
# define SHADER_HH_


# include <Utils/OpenGL.hh>
# include <iostream>
# include <map>
# include <array>
# include <vector>
# include <utility>
# include <memory>
# include <fstream>

namespace OpenGLTools
{

	class Shader
	{
	private:
		GLuint	_progId;
		GLuint	_vertexId;
		GLuint	_fragId;
		GLuint	_geometryId;
		GLuint	_computeId;
		std::map<std::string, std::vector<std::string>> _uniformBlockBind;
		std::vector<std::string> _samplersBind;
		std::vector<std::string> _uniformsBind;

		Shader();
		void compileShader(GLuint shaderId, std::string &&file) const;
		void linkProgram() const;
		GLuint addShader(std::string &&path, GLenum type);

	public:
		Shader(std::string &&compute);
		Shader(std::string &&vertex, std::string &&fragment);
		Shader(std::string &&vertex, std::string &&fragment, std::string &&geometry);
		~Shader(void);
		Shader(Shader &&Shader);
		Shader(Shader const &Shader);
		Shader &operator=(Shader const &shader);
		Shader &operator=(Shader &&shader);

		void	use();
		void	addSampler(std::string &&sampler);
		bool	deleteSampler(std::string &&sampler);
		void	addUniform(std::string &&uniform);
		bool	deleteUniform(std::string &&uniform);
		void	addUniformBlock(std::string &&uniformBlock, std::vector<std::string> &&args);
		bool	deleteUniformBlock(std::string &&unifromBlock);
	
		GLuint	getId() const;
	};

}

#endif /*!SHADER_HH_*/
