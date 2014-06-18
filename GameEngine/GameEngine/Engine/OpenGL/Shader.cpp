#include "Shader.hh"
#include <string>

namespace OpenGLTools
{
	Shader::Shader()
	:	_progId(0),
		_vertexId(0),
		_fragId(0),
		_geometryId(0),
		_computeId(0)
	{
	}

	Shader::Shader(std::string &&compute)
	{
		_computeId = addShader(std::move(compute), GL_COMPUTE_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _computeId);
		linkProgram();
	}

	Shader::Shader(std::string &&vertex, std::string &&fragment)
		: Shader()
	{
		_vertexId = addShader(std::move(vertex), GL_VERTEX_SHADER);
		_fragId = addShader(std::move(fragment), GL_FRAGMENT_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _vertexId);
		glAttachShader(_progId, _fragId);
		linkProgram();
	}

	Shader::Shader(std::string &&vertex, std::string &&fragment, std::string &&geometry)
		: Shader()
	{
		_vertexId = addShader(std::move(vertex), GL_VERTEX_SHADER);
		_fragId = addShader(std::move(fragment), GL_FRAGMENT_SHADER);
		_geometryId = addShader(std::move(geometry), GL_GEOMETRY_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _vertexId);
		glAttachShader(_progId, _fragId);
		glAttachShader(_progId, _geometryId);
		linkProgram();
	}

	Shader::Shader(Shader &&shader)
		: _progId(shader._progId),
		_vertexId(shader._vertexId),
		_fragId(shader._fragId),
		_geometryId(shader._geometryId),
		_computeId(shader._computeId),
		_uniformBlockBind(shader._uniformBlockBind),
		_samplersBind(shader._samplersBind),
		_uniformsBind(shader._uniformsBind)
	{
	}

	Shader::Shader(Shader const &shader)
		: _progId(shader._progId),
		_vertexId(shader._vertexId),
		_fragId(shader._fragId),
		_geometryId(shader._geometryId),
		_computeId(shader._computeId),
		_uniformBlockBind(shader._uniformBlockBind),
		_samplersBind(shader._samplersBind),
		_uniformsBind(shader._uniformsBind)
	{
	}

	Shader::~Shader()
	{
		glDetachShader(_progId, _vertexId);
		glDetachShader(_progId, _fragId);
		if (_geometryId)
		{
			glDetachShader(_progId, _geometryId);
			glDeleteShader(_geometryId);
		}
		if (_computeId)
		{
			glDetachShader(_progId, _computeId);
			glDeleteShader(_computeId);
		}
		glDeleteShader(_vertexId);
		glDeleteShader(_fragId);
		glDeleteProgram(_progId);
	}

	GLuint Shader::addShader(std::string &&path, GLenum type)
	{
		GLuint shaderId;
		std::ifstream file(path.c_str(), std::ios_base::binary);
		std::vector<GLchar> content;
		GLint fileSize;

		if (file.fail())
		{
			std::cerr << std::string("Error: cannot find the file " + path + ".") << std::endl;
		}
		file.seekg(0, file.end);
		fileSize = static_cast<GLint>(file.tellg()) + 1;
		file.seekg(0, file.beg);
		content = std::vector<GLchar>(fileSize);
		file.read(content.data(), fileSize - 1);
		content[fileSize - 1] = 0;
		shaderId = glCreateShader(type);
		GLchar *toShader = content.data();
		glShaderSource(shaderId, 1, const_cast<const GLchar**>(&toShader), const_cast<const GLint*>(&fileSize));
		compileShader(shaderId, std::move(path));
		return (shaderId);
	}

	void Shader::compileShader(GLuint shaderId, std::string &&file) const
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

	void Shader::addUniformBlock(std::string  &&uniformBlock, std::vector<std::string> &&args)
	{
		_uniformBlockBind[uniformBlock] = std::vector<std::string>(args);
	}

	bool Shader::deleteUniformBlock(std::string &&uniformBlock)
	{
		if (_uniformBlockBind.find(uniformBlock) == _uniformBlockBind.end())
			return (false);
		_uniformBlockBind.erase(uniformBlock);
		return (true);
	}

	void Shader::addSampler(std::string &&sampler)
	{
		_samplersBind.push_back(sampler);
		GLuint location = glGetUniformLocation(_progId, sampler.c_str());
		glUniform1ui(location, GLuint(_samplersBind.size() - 1));
	}

	bool Shader::deleteSampler(std::string &&sampler)
	{
		std::vector<std::string>::iterator it = std::find(_samplersBind.begin(), _samplersBind.end(), sampler);
		if (it == _samplersBind.end())
			return (false);
		_samplersBind.erase(it);
		GLuint location = glGetUniformLocation(_progId, sampler.c_str());
		glUniform1ui(location, (int)(it - _samplersBind.begin()));
		return (true);
	}

	void Shader::addUniform(std::string &&uniform)
	{
		_uniformsBind.push_back(uniform);
	}

	bool Shader::deleteUniform(std::string &&uniform)
	{
		std::vector<std::string>::iterator it = std::find(_uniformsBind.begin(), _uniformsBind.end(), uniform);
		if (it == _uniformsBind.end())
			return (false);
		_uniformsBind.erase(it);
		return (true);
	}

	Shader &Shader::operator=(Shader const &other)
	{
		_progId = other._progId;
		_vertexId = other._vertexId;
		_fragId = other._fragId;
		_geometryId = other._geometryId;
		_uniformBlockBind = other._uniformBlockBind;
		_samplersBind = other._samplersBind;
		_uniformsBind = other._uniformsBind;
		return (*this);
	}

	Shader &Shader::operator=(Shader &&other)
	{
		_progId = other._progId;
		_vertexId = other._vertexId;
		_fragId = other._fragId;
		_geometryId = other._geometryId;
		_uniformBlockBind = other._uniformBlockBind;
		_samplersBind = other._samplersBind;
		_uniformsBind = other._uniformsBind;
		return (*this);
	}

	GLuint Shader::getId() const
	{
		return (_progId);
	}
}