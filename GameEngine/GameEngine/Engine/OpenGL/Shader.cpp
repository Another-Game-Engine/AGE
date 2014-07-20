#include <OpenGL/Shader.hh>
#include <string>
#include <fstream>
#include <OpenGL/Texture.hh>
#include <cassert>
#include <OpenGL/UniformBlock.hh>
#include <OpenGL/OpenGLTask.hh>

# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }


namespace gl
{
	Shader::Shader()
		: _progId(0),
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

			if (_vertexName != "") { _vertexId = addShader(_vertexName, GL_VERTEX_SHADER); glAttachShader(_progId, _vertexId); }
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

	GLuint Shader::getUniformLocation(char const *flag)
	{
		GLuint location;

		use();
		if ((location = glGetUniformLocation(_progId, flag)) == -1)
			DEBUG_MESSAGE("Error", "Shader - getUniformLocation", "the location [" + std::string(flag) + "] doesn't exist on the shader", -1);
		return (location);
	}

	GLuint Shader::getUniformBlockLocation(char const *flag)
	{
		GLuint location;
		
		use();
		if ((location = glGetUniformBlockIndex(_progId, flag)) == -1)
			DEBUG_MESSAGE("Error", "Shader - getUniformBlockLocation", "the location [" + std::string(flag) + "] doesn't exist on the shader", -1);
		return (location);
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

	Shader &Shader::rmUniform(Key<Uniform> &key)
	{
		if (getUniform(key, "rmUniform") == NULL)
			return (*this);
		_uniforms.erase(key);
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
		auto &task = _uniforms[key];
		task.nbrParams = 2;
		task.func = setUniformMat4;
		setTaskAllocation(task, getUniformLocation(flag.c_str()), value);
		return (key);
	}
	
	Key<Uniform> Shader::addUniform(std::string const &flag, glm::mat3 const &value)
	{
		Key<Uniform> key;
		auto &task = _uniforms[key];
		task.nbrParams = 2;
		task.func = setUniformMat3;
		setTaskAllocation(task, getUniformLocation(flag.c_str()), value);
		return (key);
	}
	
	Key<Uniform> Shader::addUniform(std::string const &flag, glm::vec4 const &value)
	{
		Key<Uniform> key;
		auto &task = _uniforms[key];
		task.nbrParams = 2;
		task.func = setUniformVec4;
		setTaskAllocation(task, getUniformLocation(flag.c_str()), value);
		return (key);
	}

	Key<Uniform> Shader::addUniform(std::string const &flag, float value)
	{
		Key<Uniform> key;
		auto &task = _uniforms[key];
		task.nbrParams = 2;
		task.func = setUniformFloat;
		setTaskAllocation(task, getUniformLocation(flag.c_str()), value);
		return (key);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, glm::mat4 const &value)
	{
		Task *task;

		if ((task = getUniform(key, "setUniform")) == NULL)
			return (*this);
		memcpy(&task->params[1], &value, sizeof(glm::mat4));
		return (*this);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, glm::mat3 const &value)
	{
		Task *task;

		if ((task = getUniform(key, "setUniform")) == NULL)
			return (*this);
		memcpy(&task->params[1], &value, sizeof(glm::mat3));
		return (*this);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, glm::vec4 const &value)
	{
		Task *task;

		if ((task = getUniform(key, "setUniform")) == NULL)
			return (*this);
		memcpy(&task->params[1], &value, sizeof(glm::vec4));
		return (*this);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, float value)
	{
		Task *task;

		if ((task = getUniform(key, "setUniform")) == NULL)
			return (*this);
		memcpy(&task->params[1], &value, sizeof(float));
		return (*this);
	}

	Key<Sampler> Shader::addSampler(std::string const &flag)
	{
		Task task;
		Key<Sampler> key;

		auto &element = _samplers[key];
		for (int index = 0; index < GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS; ++index)
		{
			if (_units[index] == false)
			{
				_units[index] = true;
				task.func = setUniformUint;
				task.nbrParams = 2;
				setTaskAllocation(task, getUniformLocation(flag.c_str()), index);
				element = task;
				return (key);
			}
		}
		DEBUG_MESSAGE("Warning", "Shader.cpp - addSampler()", "You have not enougth texture unit for this sampler", Key<Sampler>(KEY_DESTROY));
	}

	Shader &Shader::rmSampler(Key<Sampler> &key)
	{
		Task *task;

		if ((task = getSampler(key, "rmSampler")) == NULL)
			return (*this);		
		_units[(int)task->params[1]] = false;
		_samplers.erase(key);
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
		Task *task;

		if ((task = getSampler(key, "setSampler")) == NULL)
			return (*this);
		glActiveTexture(GL_TEXTURE0 + (int)(task->params[1]));
		texture.bind();
		return (*this);
	}

	Key<InterfaceBlock> Shader::addInterfaceBlock(std::string const &flag, UniformBlock const &uniformBlock)
	{
		Key<InterfaceBlock> key;

		auto &task = _interfaceBlock[key];
		task.nbrParams = 3;
		setTaskAllocation(task, _progId, getUniformBlockLocation(flag.c_str()), uniformBlock.getBindingPoint());
		task.func = setBlockPointerUBO;
		return (key);
	}

	Shader &Shader::rmInterfaceBlock(Key<InterfaceBlock> &key)
	{
		if ((getInterfaceBlock(key, "rmInterfaceBlock")) == NULL)
			return (*this);
		_interfaceBlock.erase(key);
		key.destroy();
		return (*this);
	}

	Key<InterfaceBlock> Shader::getInterfaceBlock(size_t target) const
	{
		if (target >= _interfaceBlock.size())
			DEBUG_MESSAGE("Warning", "Shader.cpp - getInterfaceBlock(size_t target)", "the target is out of range", Key<InterfaceBlock>(KEY_DESTROY))
			auto &element = _interfaceBlock.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Task *Shader::getUniform(Key<Uniform> const &key, std::string const &msg)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.hh - " + msg, "key destroy use", NULL);
		auto &element = _uniforms.find(key);
		if (element == _uniforms.end())
			DEBUG_MESSAGE("Warning", "Shader.cpp - " + msg, "the key correspond of any element in list", NULL);
		return (&element->second);
	}

	Task *Shader::getSampler(Key<Sampler> const &key, std::string const &msg)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.hh - " + msg, "key destroy use", NULL);
		auto &element = _samplers.find(key);
		if (element == _samplers.end())
			DEBUG_MESSAGE("Warning", "Shader.cpp - " + msg, "the key correspond of any element in list", NULL);
		return (&element->second);
	}

	Task *Shader::getInterfaceBlock(Key<InterfaceBlock> const &key, std::string const &msg)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.hh - " + msg, "key destroy use", NULL);
		auto &element = _interfaceBlock.find(key);
		if (element == _interfaceBlock.end())
			DEBUG_MESSAGE("Warning", "Shader.cpp - " + msg, "the key correspond of any element in list", NULL);
		return (&element->second);
	}

}