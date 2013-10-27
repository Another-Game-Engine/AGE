//
// Shader.hh for  in /home/massora/GIT/amd_project/render/ShaderTool
// 
// Made by dorian pinaud
// Login   <pinaud_d@epitech.net>
// 
// Started on  Mon Aug  5 00:33:58 2013 dorian pinaud
// Last update Sun Aug 18 17:17:51 2013 dorian pinaud
//

#ifndef SHADER_HH_
# define SHADER_HH_

# include "Utils/OpenGL.hh"
# include "OpenGL/UniformBuffer.hh"

# include "glm/glm.hpp"

# include <iostream>
# include <map>
# include <vector>
# include <fstream>
# include <string>

namespace OpenGLTools
{

// This class is templated on the number of uniformBuffers contained
class Shader
{
private:
	GLuint						_progId;
	GLuint						_vertexId;
	GLuint						_fragId;

	void linkProgram(GLuint progId) const;
	void compileShader(GLuint shaderId, std::string const &file) const;
	GLuint addShader(std::string const &path, GLenum type);

public:
	Shader(void);
	~Shader(void);

	bool	init(std::string const &vertex, std::string const &fragment);
	bool	bindUniformBlock(std::string const &blockName, UniformBuffer const &buff);
	Shader	&bindActiveTexture(std::string const &uniformName, GLuint activeTexture);

	void	use(void);
	GLuint	getId() const;
};

}

#endif /*!SHADER_HH_*/
