#include "Utils/OpenGL.hh"

#include "Shader.hh"
#include <Utils/File.hpp>
#include <assert.h>

namespace OpenGLTools
{
	Shader::Shader()
	:	_progId(0),
		_vertexId(0),
		_fragId(0),
		_geometryId(0)
	{
	}

	Shader::Shader(std::string &&vertex, std::string &&fragment)
		: Shader()
	{
		addShader(std::move(vertex), GL_VERTEX_SHADER);
		addShader(std::move(fragment), GL_FRAGMENT_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _vertexId);
		glAttachShader(_progId, _fragId);
	}

	Shader::Shader(std::string &&vertex, std::string &&fragment, std::string &&geometry)
		: Shader()
	{
		addShader(std::move(vertex), GL_VERTEX_SHADER);
		addShader(std::move(fragment), GL_FRAGMENT_SHADER);
		addShader(std::move(geometry), GL_GEOMETRY_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _vertexId);
		glAttachShader(_progId, _fragId);
		glAttachShader(_progId, _geometryId);
		linkProgram();
	}

	Shader::~Shader()
	{
	}

	GLuint Shader::addShader(std::string &&path, GLenum type)
	{
		GLuint shaderId;
		std::ifstream file(path.c_str(), std::ios_base::binary);
		GLchar *content;
		GLint fileSize;

		if (file.fail())
		{
			std::cerr << "Error : [" << path << "] does not exists." << std::endl;
			_valid = false;
			return;
		}
		file.seekg(0, file.end);
		fileSize = static_cast<GLint>(file.tellg()) + 1;
		file.seekg(0, file.beg);
		content = new GLchar[fileSize];
		file.read(content, fileSize - 1);
		content[fileSize - 1] = 0;
		shaderId = glCreateShader(type);
		glShaderSource(shaderId, 1, const_cast<const GLchar**>(&content), const_cast<const GLint*>(&fileSize));
		compileShader(shaderId, path);
		delete[] content;
		return (shaderId);
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
			delete[] errorMsg;
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
			delete[] errorMsg;
		}
	}

bool Shader::init(std::string const &vertex, std::string const &fragment, std::string const &geometry)
{
	if (!File(vertex).exists() || !File(fragment).exists())
	{
		std::cerr << "Error : [" << vertex << "] and/or [" << fragment << "] does not exists." << std::endl;
		return false;
	}
	if (!geometry.empty() && !File(geometry).exists())
	{
		std::cerr << "Error : [" << geometry << "] does not exists." << std::endl;
		return false;
	}
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
  if (geometry.empty() == false &&
	  (_geometryId = addShader(geometry, GL_GEOMETRY_SHADER)) == 0)
    {
      std::cerr << "Error: geometry shader invalid" << std::endl;
      return (false);
    }
  _progId = glCreateProgram();
  glAttachShader(_progId, _vertexId);
  glAttachShader(_progId, _fragId);
  if (_geometryId != 0)
  {
	  glAttachShader(_progId, _geometryId);
  }
  linkProgram();

  glDetachShader(_progId, _vertexId);
  glDetachShader(_progId, _fragId);
  if (_geometryId != 0)
	  glDetachShader(_progId, _geometryId);
  glDeleteShader(_vertexId);
  glDeleteShader(_fragId);
  if (_geometryId != 0)
	  glDeleteShader(_geometryId);

  return (true);
}

GLenum  *Shader::getTargets() const
{
	return _targets;
}

std::size_t Shader::getTargetsNumber() const
{
	return _targetsList.size();
}

bool Shader::_build()
{
	use();

	// if there is no targets defined
	if (_targetsList.size() == 0)
	{
		std::cout << "No targets defined for shader" << std::endl;
		_targetsList.insert(GL_COLOR_ATTACHMENT0);
	}

	if (_targets)
		delete _targets;
	_targets = new GLenum[_targetsList.size()];

	unsigned int i = 0;
	for (auto &e : _targetsList)
	{
#pragma warning(suppress: 6386)
		_targets[i] = static_cast<GLenum>(e);
		++i;
	}

	unsigned int destIndex = 0;
	for (auto &e : _layersList)
	{
		GLuint	location = glGetUniformLocation(_progId, std::string("layer" + std::to_string(destIndex)).c_str());

		glUniform1i(location, destIndex);
		if (glGetError() != GL_NO_ERROR)
			std::cerr << "Bind active texture failed for uniform <layer" << e << ">." << std::endl;
		++destIndex;
	}

	for (unsigned int it = 0; it < _textureNumber; ++it)
	{
		GLuint	location = glGetUniformLocation(_progId, std::string("fTexture" + std::to_string(it)).c_str());

		glUniform1i(location, it);
		if (glGetError() != GL_NO_ERROR)
			std::cerr << "Bind active texture failed for uniform <fTexture" << it << ">." << std::endl;
		++destIndex;
	}

	return true;
}

	Shader &Shader::addTarget(GLenum target)
	{
		_targetsList.insert(target);
		return *this;
	}

	Shader &Shader::removeTarget(GLenum target)
	{
		_targetsList.erase(target);
		return *this;
	}

	void Shader::clearTargets()
	{
		_targetsList.clear();
		if (_targets)
			delete _targets;
	}

	Shader &Shader::addLayer(GLenum layer)
	{
		_layersList.insert(layer);
		return *this;
	}

	Shader &Shader::removeLayer(GLenum layer)
	{
		_layersList.erase(layer);
		return *this;
	}

	void Shader::clearLayers()
	{
		_layersList.clear();
	}

}