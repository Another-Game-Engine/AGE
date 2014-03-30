#ifndef SHADER_HH_
# define SHADER_HH_

# include <map>
# include <array>
# include <vector>
# include <set>

#include "AShader.hh"

namespace OpenGLTools
{
	class Shader : public AShader
	{
	private:
		GLuint	_progId;
		GLuint	_vertexId;
		GLuint	_fragId;
		GLuint	_geometryId;
		bool	_valid;

		Shader();
		void compileShader(GLuint shaderId, std::string const &file) const;
		void linkProgram() const;
		GLuint addShader(std::string &&path, GLenum type);

	public:
		Shader(std::string &&vertex, std::string &&fragment);
		Shader(std::string &&vertex, std::string &&fragment, std::string &&geometry);
		~Shader(void);
		Shader(Shader &&Shader);
		Shader(Shader const &Shader);
		Shader &operator=(Shader const &shader);
		Shader &operator=(Shader &&shader);

		bool	addSampler(std::string &&sampler);
		bool	deleteSampler(std::string &&sampler);
		bool	addUniform(std::string &&uniform);
		bool	deleteUniform(std::string &&uniform);
		template <typename T>
		bool	addUniformBlock(std::string &&uniformBlock, std::array<T, std::string> &&args);
		void	deleteUniformBlock(std::string &&unifromBlock);
		bool	isValid() const;
		bool	operator!() const;
	};

}

#endif /*!SHADER_HH_*/
