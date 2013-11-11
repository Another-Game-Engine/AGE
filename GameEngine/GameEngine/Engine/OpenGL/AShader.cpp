#include "AShader.hh"

using namespace OpenGLTools;

AShader::AShader()
	: _progId(0)
{}

AShader::~AShader()
{
	glDeleteProgram(_progId);
}

bool	AShader::bindUniformBlock(std::string const &blockName, UniformBuffer const &buffer)
{
	GLuint	blockId;

	assert(_progId != 0 && "Must initialize the shader...");
	blockId = glGetUniformBlockIndex(_progId, blockName.c_str());
	glUniformBlockBinding(_progId, blockId, buffer.getBindingPoint());
	return (true);
}

void AShader::compileShader(GLuint shaderId, std::string const &file) const
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

void AShader::linkProgram() const
{
  GLint         linkRet = 0;
  GLsizei       msgLenght;
  GLchar        *errorMsg;

  glLinkProgram(_progId);
  glGetProgramiv(_progId, GL_LINK_STATUS, &linkRet);
  if (linkRet == GL_FALSE)
    {
      glGetProgramiv(_progId, GL_INFO_LOG_LENGTH, &msgLenght);
      errorMsg = new GLchar[msgLenght];
      glGetProgramInfoLog(_progId, msgLenght,
			  &msgLenght, errorMsg);
      std::cerr << "Link error on program : " << std::endl;
	  std::cerr << std::string(errorMsg).data() << std::endl;
      delete [] errorMsg;
    }
}

/// link the program with the shaders
GLuint AShader::addShader(std::string const &path, GLenum type)
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
void AShader::use(void)
{
  glUseProgram(_progId);
}

GLuint	AShader::getId() const
{
	return (_progId);
}

