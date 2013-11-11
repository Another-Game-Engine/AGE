#ifndef   __ASHADER_HH__
# define  __ASHADER_HH__

# include "Utils/OpenGL.hh"
# include "OpenGL/UniformBuffer.hh"
# include "glm/glm.hpp"

# include <iostream>
# include <fstream>
# include <string>


namespace OpenGLTools
{
	class AShader
	{
	public:
		AShader();
		virtual ~AShader();
		bool	bindUniformBlock(std::string const &blockName, UniformBuffer const &buff);
		bool build() {return _build();}
		void	use(void);
		GLuint	getId() const;
	protected:
		void compileShader(GLuint shaderId, std::string const &file) const;
		void linkProgram(GLuint progId) const;
		GLuint addShader(std::string const &path, GLenum type);
	private:
		virtual bool _build() = 0;
	protected:
		GLuint						_progId;
	};
};
#endif    //__ASHADER_HH__
