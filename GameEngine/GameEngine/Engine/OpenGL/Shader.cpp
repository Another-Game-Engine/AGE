//
// Shader.cpp for  in /home/massora/GIT/amd_project/render/ShaderTool
// 
// Made by dorian pinaud
// Login   <pinaud_d@epitech.net>
// 
// Started on  Mon Aug  5 00:37:46 2013 dorian pinaud
// Last update Mon Aug 19 00:15:12 2013 dorian pinaud
//

#include "Utils/OpenGL.hh"

#include "Shader.hh"
#include <assert.h>

namespace OpenGLTools
{

Shader::Shader(void)
{
}

Shader::~Shader(void)
{
}

bool Shader::init(std::string const &vertex, std::string const &fragment)
{
  if ((_vertexId = addShader(vertex, GL_VERTEX_SHADER)) == 0)
    {
      std::cerr << "Error: vertex shader invalid" << std::endl;
      return (false);
    }
  if ((_fragId = addShader(fragment, GL_FRAGMENT_SHADER)) == 0)
    {
      std::cerr << "Error: fragment shader invalid" << std::endl;
      return (false);
    }
  _progId = glCreateProgram();
  glAttachShader(_progId, _vertexId);
  glAttachShader(_progId, _fragId);
  linkProgram(_progId);
  return (true);
}

void Shader::use(void)
{
  glUseProgram(_progId);
}

/// link the program with the shaders
GLuint Shader::addShader(std::string const &path, GLenum type)
{
  GLuint shaderId;
  std::ifstream file(path.c_str(), std::ios_base::binary);
  GLchar *content;
  GLint fileSize;

  if (file.fail())
    return (0);
  file.seekg(0, file.end);
  fileSize = static_cast<GLint>(file.tellg()) + 1;
  file.seekg(0, file.beg);
  content = new GLchar[fileSize];
  file.read(content, fileSize - 1);
  content[fileSize - 1] = 0;
  shaderId = glCreateShader(type);
  glShaderSource(shaderId, 1,
                 const_cast<const GLchar**>(&content),
                 const_cast<const GLint*>(&fileSize));
  compileShader(shaderId, path);
  delete [] content;
  return (shaderId);
}

void Shader::linkProgram(GLuint progId) const
{
  GLint         linkRet = 0;
  GLsizei       msgLenght;
  GLchar        *errorMsg;

  glLinkProgram(progId);
  glGetProgramiv(progId, GL_LINK_STATUS, &linkRet);
  if (linkRet == GL_FALSE)
    {
      glGetProgramiv(progId, GL_INFO_LOG_LENGTH, &msgLenght);
      errorMsg = new GLchar[msgLenght];
      glGetProgramInfoLog(progId, msgLenght,
			  &msgLenght, errorMsg);
      std::cerr << "Link error on program : " << std::endl;
	  std::cerr << std::string(errorMsg).data() << std::endl;
      delete [] errorMsg;
    }
}

void Shader::compileShader(GLuint shaderId, std::string const &file) const
{
  GLint         compileRet = 0;
  GLsizei       msgLenght;
  GLchar        *errorMsg;

  glCompileShader(shaderId);
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileRet);
  if (compileRet == GL_FALSE)
    {
      glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &msgLenght);
      errorMsg = new GLchar[msgLenght];
      glGetShaderInfoLog(shaderId, msgLenght,
			 &msgLenght, errorMsg);
      std::cerr << "Compile error on " << file.data() << ": " << std::endl;
      std::cerr << errorMsg << std::endl;
      delete [] errorMsg;
    } 
}

bool	Shader::bindUniformBlock(std::string const &blockName, UniformBuffer<> const &buffer)
{
	GLuint	blockId;

	assert(_progId != 0 && "Must initialize the shader...");
	blockId = glGetUniformBlockIndex(_progId, blockName.c_str());
	glUniformBlockBinding(_progId, blockId, buffer.getBindingPoint());
	return (true);
}

GLuint	Shader::getId() const
{
	return (_progId);
}

}