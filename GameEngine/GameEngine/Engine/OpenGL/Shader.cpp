#include "Shader.hh"
#include <string>
# include <fstream>

# define DEBUG_MESSAGE(type, from, reason, return_type) \
{	std::cerr << std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "]." << std::endl; return return_type; }

namespace gl
{
	Shader::Shader()
	:	_progId(0),
		_vertexId(0),
		_fragId(0),
		_geometryId(0),
		_computeId(0),
		_vertexName(""),
		_fragName(""),
		_geometryName(""),
		_computeName("")
	{
	}

	Shader::Shader(std::string const &compute)
		: Shader()
	{
		_computeName = compute;
		_computeId = addShader(compute, GL_COMPUTE_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _computeId);
		linkProgram();
	}

	Shader::Shader(std::string const &vertex, std::string const &fragment)
		: Shader()
	{
		_vertexName = vertex;
		_fragName = fragment;
		_vertexId = addShader(vertex, GL_VERTEX_SHADER);
		_fragId = addShader(fragment, GL_FRAGMENT_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _vertexId);
		glAttachShader(_progId, _fragId);
		linkProgram();
	}

	Shader::Shader(std::string const &vertex, std::string const &fragment, std::string const &geometry)
		: Shader()
	{
		_vertexName = vertex;
		_fragName = fragment;
		_geometryName = geometry;
		_vertexId = addShader(vertex, GL_VERTEX_SHADER);
		_fragId = addShader(fragment, GL_FRAGMENT_SHADER);
		_geometryId = addShader(geometry, GL_GEOMETRY_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _vertexId);
		glAttachShader(_progId, _fragId);
		glAttachShader(_progId, _geometryId);
		linkProgram();
	}

	Shader::Shader(Shader const &shader)
		: Shader()
	{
		_vertexName = shader._vertexName;
		_fragName = shader._fragName;
		_geometryName = shader._geometryName;
		_vertexId = addShader(_vertexName, GL_VERTEX_SHADER);
		_fragId = addShader(_fragName, GL_FRAGMENT_SHADER);
		_geometryId = addShader(_geometryName, GL_GEOMETRY_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _vertexId);
		glAttachShader(_progId, _fragId);
		glAttachShader(_progId, _geometryId);
		linkProgram();
	}

	Shader &Shader::operator=(Shader const &s)
	{
		if (this != &s)
		{
			if (_progId > 0)
			{
				if (_vertexId > 0) { glDetachShader(_progId, _vertexId); glDeleteShader(_vertexId); }
				if (_geometryId > 0) { glDetachShader(_progId, _geometryId); glDeleteShader(_geometryId); }
				if (_computeId > 0) { glDetachShader(_progId, _computeId); glDeleteShader(_computeId); }
				if (_fragId > 0) { glDetachShader(_progId, _fragId); glDeleteShader(_fragId); }
				glDeleteProgram(_progId);
			}
			_vertexName = s._vertexName;
			_fragName = s._fragName;
			_geometryName = s._geometryName;
			_vertexId = addShader(_vertexName, GL_VERTEX_SHADER);
			_fragId = addShader(_fragName, GL_FRAGMENT_SHADER);
			_geometryId = addShader(_geometryName, GL_GEOMETRY_SHADER);
			_progId = glCreateProgram();
			glAttachShader(_progId, _vertexId);
			glAttachShader(_progId, _fragId);
			glAttachShader(_progId, _geometryId);
			linkProgram();
		}
		return (*this);
	}

	Shader::~Shader()
	{
		if (_progId > 0)
		{
			if (_vertexId > 0) { glDetachShader(_progId, _vertexId); glDeleteShader(_vertexId); }
			if (_geometryId > 0) { glDetachShader(_progId, _geometryId); glDeleteShader(_geometryId); }
			if (_computeId > 0) { glDetachShader(_progId, _computeId); glDeleteShader(_computeId); }
			if (_fragId > 0) { glDetachShader(_progId, _fragId); glDeleteShader(_fragId); }
			glDeleteProgram(_progId);
		}
	}

	GLuint Shader::addShader(std::string const &path, GLenum type)
	{
		GLuint shaderId;
		std::ifstream file(path.c_str(), std::ios_base::binary);
		GLchar *content;
		GLint fileSize;

		if (file.fail())
			DEBUG_MESSAGE("Error", "Shader.cpp-Shader(path, type)", "File doesn't exist", -1);
		file.seekg(0, file.end);
		fileSize = static_cast<GLint>(file.tellg()) + 1;
		file.seekg(0, file.beg);
		content = new GLchar[fileSize];
		file.read(content, fileSize - 1);
		content[fileSize - 1] = 0;
		shaderId = glCreateShader(type);
		glShaderSource(shaderId, 1, const_cast<const GLchar**>(&content), const_cast<const GLint*>(&fileSize));
		compileShader(shaderId, path);
		return (shaderId);
	}

	void Shader::compileShader(GLuint shaderId, std::string const &file) const
	{
		GLint         compileRet = 0;
		GLsizei       msgLenght;
		GLchar        *errorMsg;

		glCompileShader(shaderId);
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileRet);
		// write error shader message
		if (compileRet == GL_FALSE)
		{
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &msgLenght);
			errorMsg = new GLchar[msgLenght];
			glGetShaderInfoLog(shaderId, msgLenght, &msgLenght, errorMsg);
			std::cerr << "Compile error on " << file.data() << ": " << std::endl;
			std::cerr << errorMsg << std::endl;
			delete[] errorMsg;
		}
	}

	void Shader::linkProgram() const
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

	void Shader::use()
	{
		glUseProgram(_progId);
	}

	GLuint Shader::getId() const
	{
		return (_progId);
	}

	bool Shader::isValid() const
	{
		if (_vertexId == -1 || _fragId == -1 || _geometryId == -1 || _computeId == -1)
			return (false);
		return (true);
	}

	std::string const &Shader::getVertexName() const
	{
		return (_vertexName);
	}

	std::string const &Shader::getFragName() const
	{
		return (_fragName);
	}

	std::string const &Shader::getGeoName() const
	{
		return (_geometryName);
	}

	std::string const &Shader::getComputeName() const
	{
		return (_computeName);
	}
}