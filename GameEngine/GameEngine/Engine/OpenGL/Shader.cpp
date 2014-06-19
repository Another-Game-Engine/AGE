#include <OpenGL/Shader.hh>
#include <string>
#include <fstream>
#include <OpenGL/Uniform.hh>
#include <OpenGL/Texture.hh>

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
		for (int index = 0; index < GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS; ++index)
			_units[index] = NULL;
	}

	Shader::Shader(std::string const &compute)
		: Shader()
	{
		_computeName = compute;
		_computeId = addShader(compute, GL_COMPUTE_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _computeId);
		if (linkProgram() == false)
			_vertexId = _fragId = _computeId = _geometryId = -1;
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
		if (linkProgram() == false)
			_vertexId = _fragId = _computeId = _geometryId = -1;
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
		if (linkProgram() == false)
			_vertexId = _fragId = _computeId = _geometryId = -1;
	}

	Shader::Shader(Shader const &shader)
		: Shader()
	{
		_uniforms = shader._uniforms;
		_samplers = shader._samplers;
		for (int index = 0; index < GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS; ++index)
			_units[index] = shader._units[index];
		_vertexName = shader._vertexName;
		_fragName = shader._fragName;
		_geometryName = shader._geometryName;
		_computeName = shader._computeName;

		_progId = glCreateProgram();

		if (_vertexName != "") { _vertexId = addShader(_vertexName, GL_VERTEX_SHADER); glAttachShader(_progId, _vertexId); }
		if (_fragName != "") { _fragId = addShader(_fragName, GL_FRAGMENT_SHADER); glAttachShader(_progId, _fragId); }
		if (_geometryName != "") { _geometryId = addShader(_geometryName, GL_GEOMETRY_SHADER); glAttachShader(_progId, _geometryId); }
		if (_computeName != "") { _computeId = addShader(_computeName, GL_COMPUTE_SHADER); glAttachShader(_progId, _computeId); }
		if (linkProgram() == false)
			_vertexId = _fragId = _computeId = _geometryId = -1;
	}

	Shader &Shader::operator=(Shader const &s)
	{
		if (this != &s)
		{
			_uniforms = s._uniforms;
			_samplers = s._samplers;
			for (int index = 0; index < GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS; ++index)
				_units[index] = s._units[index];
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
			_computeName = s._computeName;
			
			_progId = glCreateProgram();

			if (_vertexName != "") { _vertexId = addShader(_vertexName, GL_VERTEX_SHADER); glAttachShader(_progId, _vertexId);}
			if (_fragName != "") { _fragId = addShader(_fragName, GL_FRAGMENT_SHADER); glAttachShader(_progId, _fragId); }
			if (_geometryName != "") { _geometryId = addShader(_geometryName, GL_GEOMETRY_SHADER); glAttachShader(_progId, _geometryId); }
			if (_computeName != "") { _computeId = addShader(_computeName, GL_COMPUTE_SHADER); glAttachShader(_progId, _computeId); }

			if (linkProgram() == false)
				_vertexId = _fragId = _computeId = _geometryId = -1;
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
		if (compileShader(shaderId, path) == false)
			DEBUG_MESSAGE("Error", "Shader.cpp-Shader(path, type)", "File doesn't compile", -1);
		return (shaderId);
	}

	bool Shader::compileShader(GLuint shaderId, std::string const &file) const
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
			std::cerr << std::endl << errorMsg << std::endl << std::endl;
			delete[] errorMsg;
			DEBUG_MESSAGE("Error", "Shader.cpp-compileShader(shaderId, file)", "File doesn't compile", false);
		}
		return (true);
	}

	bool Shader::linkProgram() const
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
			std::cerr << std::endl << errorMsg << std::endl << std::endl;
			delete[] errorMsg;
			DEBUG_MESSAGE("Error", "Shader.cpp-linkProgram()", "File doesn't link", false);
		}
		return (true);
	}

	void Shader::use() const
	{
		static GLint idbind = 0;
		
		if (idbind != _progId)
		{
			glUseProgram(_progId);
			idbind = _progId;
		}
	}

	GLuint Shader::getId() const
	{
		return (_progId);
	}

	bool Shader::isValid() const
	{
		if (_vertexId == -1 || _fragId == -1 || _geometryId == -1 || _computeId == -1)
			return (false);
		if ((_vertexId + _fragId + _geometryId + _computeId) == 0)
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

	Key<Uniform> Shader::addUniform(std::string const &flag)
	{
		Key<Uniform> key;

		_uniforms[key] = Uniform(flag, this);
		return (key);
	}

	Shader &Shader::rmUniform(Key<Uniform> &key)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.cpp - rmUniform", "the key is destroy", *this);
		auto element = _uniforms.find(key);
		if (element == _uniforms.end())
			DEBUG_MESSAGE("Warning", "Shader.cpp - rmUniform", "the key correspond of any element in uniform list", *this);
		_uniforms.erase(element);
		key.destroy();
		return (*this);
	}

	Key<Uniform> Shader::getUniform(size_t target) const
	{
		if (target >= _uniforms.size())
			DEBUG_MESSAGE("Warning", "Shader.cpp - getUniform(size_t target)", "the target is out of range", Key<Uniform>(KEY_DESTROY))
		auto &element = _uniforms.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<Uniform> Shader::addUniform(std::string const &flag, glm::mat4 const &value)
	{
		Key<Uniform> key;

		auto &element = _uniforms[key] = Uniform(flag, this);
		element.set(value);
		return (key);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, glm::mat4 const &value)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.hh - setUniform(key, value)", "key destroy use", *this);
		auto &element = _uniforms.find(key);
		if (element == _uniforms.end())
			DEBUG_MESSAGE("Warning", "Shader.hh - setUniform(key, value)", "element in not find", *this);
		element->second.set(value);
		return (*this);
	}

	Key<Sampler> Shader::addSampler(std::string const &flag)
	{
		Key<Sampler> key;

		auto &element = _samplers[key] = Uniform(flag, this);
		for (int index = 0; index < GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS; ++index)
		{
			if (_units[index] == false)
			{
				_units[index] = true;
				element.set(index);
				return (key);
			}
		}
		DEBUG_MESSAGE("Warning", "Shader.cpp - addSampler()", "You have not enougth texture unit for this sampler", Key<Sampler>(KEY_DESTROY));
	}

	Shader &Shader::rmSampler(Key<Sampler> key)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.cpp - rmSampler", "the key is destroy", *this);
		auto element = _samplers.find(key);
		if (element == _samplers.end())
			DEBUG_MESSAGE("Warning", "Shader.cpp - rmSampler", "the key correspond of any element in samplers list", *this);
		_units[element->second.get<int>()] = false;
		_samplers.erase(element);
		key.destroy();
		return (*this);
	}

	Key<Sampler> Shader::getSampler(size_t target) const
	{
		if (target >= _samplers.size())
			DEBUG_MESSAGE("Warning", "Shader.cpp - getSampler(size_t target)", "the target is out of range", Key<Sampler>(KEY_DESTROY))
		auto &element = _samplers.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Shader &Shader::setSampler(Key<Sampler> const &key, Texture const &texture)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.hh - setSampler(key, value)", "key destroy use", *this);
		auto element = _samplers.find(key);
		if (element == _samplers.end())
			DEBUG_MESSAGE("Warning", "Shader.cpp - setSampler", "the key correspond of any element in samplers list", *this);
		glActiveTexture(GL_TEXTURE0 + element->second.get<int>());
		texture.bind();
	}

}