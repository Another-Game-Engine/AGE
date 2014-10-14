#include <Render/Shader.hh>
#include <string>
# include <fstream>
#include <cassert>

#include <Render/Task.hh>
#include <Render/PreShader.cpp>


namespace gl
{

	Shader::Shader()
		: _unitProgId(NULL),
		_progId(-1),
		_nbrUnitProgId(0),
		_bindTransformation(Key<Uniform>::createKey())
	{

	}

	Shader *Shader::createShader(std::string const &v, std::string const &f, std::string const &g)
	{
		Shader *s = new Shader;

		s->_nbrUnitProgId = 3;
		s->_unitProgId = new GLuint[s->_nbrUnitProgId];
		if ((s->_unitProgId[0] = addUnitProgByFile(v, GL_VERTEX_SHADER)) == -1)
			return (NULL);
		if ((s->_unitProgId[1] = addUnitProgByFile(f, GL_FRAGMENT_SHADER)) == -1)
			return (NULL);
		if ((s->_unitProgId[2] = addUnitProgByFile(g, GL_GEOMETRY_SHADER)) == -1)
			return (NULL);
		if (s->createProgram() == false)
			return (NULL);
		return (s);
	}

	Shader *Shader::createShader(std::string const &v, std::string const &f)
	{
		Shader *s = new Shader;

		s->_nbrUnitProgId = 2;
		s->_unitProgId = new GLuint[s->_nbrUnitProgId];
		if ((s->_unitProgId[0] = addUnitProgByFile(v, GL_VERTEX_SHADER)) == -1)
			return (NULL);
		if ((s->_unitProgId[1] = addUnitProgByFile(f, GL_FRAGMENT_SHADER)) == -1)
			return (NULL);
		if (s->createProgram() == false)
			return (NULL);
		return (s);
	}

	Shader *Shader::createComputeShader(std::string const &c)
	{
		Shader *s = new Shader;

		s->_nbrUnitProgId = 1;
		s->_unitProgId = new GLuint[s->_nbrUnitProgId];
		if ((s->_unitProgId[0] = addUnitProgByFile(c, GL_VERTEX_SHADER)) == -1)
			return (NULL);
		if (s->createProgram() == false)
			return (NULL);
		return (s);
	}

	Shader *Shader::createPreShaderQuad()
	{
		Shader *s = new Shader;

		s->_nbrUnitProgId = 2;
		s->_unitProgId = new GLuint[s->_nbrUnitProgId];
		if ((s->_unitProgId[0] = addUnitProg(quad_shader_vertex, GL_VERTEX_SHADER)) == -1)
			return (NULL);
		if ((s->_unitProgId[1] = addUnitProg(quad_shader_fragment, GL_FRAGMENT_SHADER)) == -1)
			return (NULL);
		if (s->createProgram() == false)
			return (NULL);
		return (s);
	}

	Shader::~Shader()
	{
		if (_nbrUnitProgId > 0)
			delete[] _unitProgId;
		for (uint8_t index = 0; index < _nbrUnitProgId; ++index)
			glDetachShader(_progId, _unitProgId[index]);
		glDeleteProgram(_progId);
	}

	bool Shader::createProgram()
	{
		_progId = glCreateProgram();
		for (uint8_t index = 0; index < _nbrUnitProgId; ++index)
			glAttachShader(_progId, _unitProgId[index]);
		return (linkProgram());
	}

	bool Shader::linkProgram() const
	{
		GLint         linkRet = 0;
		GLint         params = 0;
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
			assert(0);
		}
		return (true);
	}

	GLuint Shader::getSamplerLocation(char const *flag)
	{
		GLuint location;

		use();
		if ((location = glGetUniformLocation(_progId, flag)) == -1)
			assert(0);
		glUniform1i(location, location);
		return (location);
	}

	GLuint Shader::getUniformLocation(char const *flag)
	{
		GLuint location;

		use();
		if ((location = glGetUniformLocation(_progId, flag)) == -1)
			assert(0);
		return (location);
	}

	GLuint Shader::getUniformBlockLocation(char const *flag)
	{
		GLuint location;
		
		use();
		if ((location = glGetUniformBlockIndex(_progId, flag)) == -1)
			assert(0);
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

	Key<Uniform> Shader::getUniform(size_t target) const
	{
		if (target >= _uniforms.size())
			assert(0);
		auto &element = _uniforms.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	void Shader::createUniformTask(Task &task, std::string const &flag)
	{
		task.type = TypeTask::UniformTask;
		task.func = NULL;
		task.nbrParams = 2;
		task.indexToTarget = 1;
		task.sizeParams = new size_t[task.nbrParams];
		task.params = new void *[task.nbrParams];
		task.params[0] = new GLuint;
		task.sizeParams[0] = sizeof(GLuint);
		GLuint location = getUniformLocation(flag.c_str());
		*(GLuint *)task.params[0] = location;
		task.params[1] = NULL;
		task.sizeParams[1] = 0;
	}

	void Shader::createSamplerTask(Task &task, std::string const &flag)
	{
		task.type = TypeTask::SamplerTask;
		task.func = setUniformSampler;
		task.indexToTarget = 2;
		task.nbrParams = 3;
		task.sizeParams = new size_t[task.nbrParams];
		task.params = new void *[task.nbrParams];
		task.params[0] = new GLuint;
		task.sizeParams[0] = sizeof(GLuint);
		GLuint location = getSamplerLocation(flag.c_str());
		*(GLuint *)task.params[0] = location;
		task.params[1] = new GLint;
		*(GLenum *)task.params[1] = GL_TEXTURE_2D;
		task.sizeParams[1] = sizeof(GLenum);
		task.params[2] = new GLint;
		*(GLint *)task.params[2] = 0;
		task.sizeParams[2] = sizeof(GLint);
	}

	void Shader::createUniformBlockTask(Task &task, std::string const &flag, UniformBlock &ubo)
	{
		task.type = TypeTask::InterfaceBlockTask;
		task.func = setBlockBinding;
		task.indexToTarget = 0;
		task.nbrParams = 3;
		task.sizeParams = new size_t[task.nbrParams];
		task.params = new void *[task.nbrParams];
		task.params[0] = new GLuint;
		*(GLuint *)task.params[0] = _progId;
		task.sizeParams[0] = sizeof(GLuint);
		task.params[1] = new GLuint;
		GLuint location = getUniformBlockLocation(flag.c_str());
		*(GLuint *)task.params[1] = location;
		task.sizeParams[1] = sizeof(GLuint);
		task.params[2] = new GLuint;
		*(GLuint *)task.params[2] = ubo.getBindingPoint();
		task.sizeParams[2] = sizeof(GLuint);
		ubo.introspection(*this, location);
	}

	Key<Uniform> Shader::addUniform(std::string const &flag)
	{
		Key<Uniform> key = Key<Uniform>::createKey();
		_tasks.push_back(Task());
		Task *task = &_tasks.back();
		_uniforms[key] = _tasks.size() - 1;
		createUniformTask(*task, flag);
		return (key);
	}

	Key<Uniform> Shader::addUniform(std::string const &flag, glm::mat4 const &value)
	{
		Key<Uniform> key = Key<Uniform>::createKey();
		_tasks.push_back(Task());
		Task *task = &_tasks[_tasks.size() - 1];
		_uniforms[key] = _tasks.size() - 1;
		createUniformTask(*task, flag);
		setUniformTask<glm::mat4>(*task, setUniformMat4, (void *)&value);
		return (key);
	}
	
	Key<Uniform> Shader::addUniform(std::string const &flag, glm::mat3 const &value)
	{
		Key<Uniform> key = Key<Uniform>::createKey();
		_tasks.push_back(Task());
		Task *task = &_tasks.back();
		_uniforms[key] = _tasks.size() - 1;
		createUniformTask(*task, flag);
		setUniformTask<glm::mat3>(*task, setUniformMat3, (void *)&value);
		return (key);
	}

	Key<Uniform> Shader::addUniform(std::string const &flag, glm::vec3 const &value)
	{
		Key<Uniform> key = Key<Uniform>::createKey();
		_tasks.push_back(Task());
		Task *task = &_tasks.back();
		_uniforms[key] = _tasks.size() - 1;
		createUniformTask(*task, flag);
		setUniformTask<glm::vec3>(*task, setUniformVec3, (void *)&value);
		return (key);
	}
	
	Key<Uniform> Shader::addUniform(std::string const &flag, glm::vec4 const &value)
	{
		Key<Uniform> key = Key<Uniform>::createKey();
		_tasks.push_back(Task());
		Task *task = &_tasks.back();
		_uniforms[key] = _tasks.size() - 1;
		createUniformTask(*task, flag);
		setUniformTask<glm::vec4>(*task, setUniformVec4, (void *)&value);		return (key);
		return (key);
	}

	Key<Uniform> Shader::addUniform(std::string const &flag, float value)
	{
		Key<Uniform> key = Key<Uniform>::createKey();
		_tasks.push_back(Task());
		Task *task = &_tasks.back();
		_uniforms[key] = _tasks.size() - 1;
		createUniformTask(*task, flag);
		setUniformTask<float>(*task, setUniformFloat, (void *)&value);
		return (key);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, glm::mat4 const &value)
	{
		Task *task = getUniform(key);
		setUniformTask<glm::mat4>(*task, setUniformMat4, (void *)&value);
		return (*this);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, glm::mat3 const &value)
	{
		Task *task = getUniform(key);
		setUniformTask<glm::mat3>(*task, setUniformMat3, (void *)&value);
		return (*this);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, glm::vec4 const &value)
	{
		Task *task = getUniform(key);
		setUniformTask<glm::vec4>(*task, setUniformVec4, (void *)&value);
		return (*this);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, glm::vec3 const &value)
	{
		Task *task = getUniform(key);
		setUniformTask<glm::vec3>(*task, setUniformVec3, (void *)&value);
		return (*this);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, float value)
	{
		Task *task = getUniform(key);
		setUniformTask<float>(*task, setUniformFloat, (void *)&value);
		return (*this);
	}

	Key<Sampler> Shader::addSampler(std::string const &flag)
	{
		Key<Sampler> key = Key<Sampler>::createKey();

		_tasks.push_back(Task());
		Task *task = &_tasks.back();
		_samplers[key] = _tasks.size() - 1;
		createSamplerTask(*task, flag);
		return (key);
	}

	Key<Sampler> Shader::getSampler(size_t target) const
	{
		if (target >= _samplers.size())
			assert(0);
		auto &element = _samplers.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Shader &Shader::setSampler(Key<Sampler> const &key, Texture const &texture)
	{
		Task *task = getSampler(key);
		setSamplerTask(*task, texture);
		return (*this);
	}

	bool Shader::hasSampler(Key<Sampler> const &key)
	{
		if (getSampler(key) != NULL)
			return (true);
		return (false);
	}

	Key<InterfaceBlock> Shader::addInterfaceBlock(std::string const &flag, UniformBlock &uniformBlock)
	{
		Key<InterfaceBlock> key = Key<InterfaceBlock>::createKey();

		_tasks.push_back(Task());
		_interfaceBlock[key] = _tasks.size() - 1;
		Task *task = &_tasks.back();
		createUniformBlockTask(*task, flag, uniformBlock);
		return (key);
	}

	Key<InterfaceBlock> Shader::getInterfaceBlock(size_t target) const
	{
		if (target >= _interfaceBlock.size())
			assert(0);
		auto &element = _interfaceBlock.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Task *Shader::getUniform(Key<Uniform> const &key)
	{
		size_t index = getIndexUniform(key);
		if (index != -1)
			return (&_tasks[index]);
		return (NULL);
	}

	size_t Shader::getIndexUniform(Key<Uniform> const &key)
	{
		if (!key)
			assert(0);
		auto &element = _uniforms.find(key);
		if (element == _uniforms.end())
			assert(0);
		return (element->second);
	}

	size_t Shader::getIndexSampler(Key<Sampler> const &key)
	{
		if (!key)
			assert(0);	
		auto &element = _samplers.find(key);
		if (element == _samplers.end())
			assert(0);	
		return (element->second);
	}

	Task *Shader::getSampler(Key<Sampler> const &key)
	{
		size_t index = getIndexSampler(key);
		if (index != -1)
			return (&_tasks[index]);
		return (NULL);
	}

	size_t Shader::getIndexInterfaceBlock(Key<InterfaceBlock> const &key)
	{
		if (!key)
			assert(0);
		auto &element = _interfaceBlock.find(key);
		if (element == _interfaceBlock.end())
			assert(0);
		return (element->second);
	}

	Task *Shader::getInterfaceBlock(Key<InterfaceBlock> const &key)
	{
		size_t index = getIndexInterfaceBlock(key);
		if (index != -1)
			return (&_tasks[index]);
		return (NULL);
	}

	size_t Shader::getUniformBindMaterial(Key<Uniform> const &key/*, std::string const &msg*/)
	{
		assert(key);
		auto &element = _bindUniform.find(key);
		assert(element != _bindUniform.end());
		return (element->second);
	}

	Shader &Shader::setInterfaceBlock(Key<InterfaceBlock> const &key, UniformBlock &uniformBlock)
	{
		Task *task;

		if ((task = getInterfaceBlock(key)) == NULL)
			return (*this);
		setUniformBlockTask(*task, uniformBlock);
		return (*this);
	}

	Shader &Shader::introspection(Key<InterfaceBlock> const &key, UniformBlock &u)
	{
		Task *task = getInterfaceBlock(key);
		u.introspection(*this, *((GLuint *)task->params[1]));
		return (*this);
	}

	void Shader::setTransformationTask(glm::mat4 const &mat)
	{
		Task *task = getUniform(_bindTransformation);
		setUniformTask<glm::mat4>(*task, setUniformMat4, (void *)&mat);
	}

	Shader &Shader::setMaterial(Material const &material)
	{
		use();
		for (size_t index = 0; index < _bindMaterial.size(); ++index)
		{
			if (_bindMaterial[index].isUse)
			{
				setTaskWithMaterial(_bindMaterial[index], material);
			}
		}
		return (*this);
	}

	Shader &Shader::update(glm::mat4 const &transform)
	{
		setTransformationTask(transform);
		update();
		return (*this);
	}

	Shader &Shader::update()
	{
		use();
		for (auto &task : _tasks)
		{
			assert(task.isExec());
			if (task.update)
			{
				task.func(task.params);
				task.update = false;
			}
		}
		return (*this);
	}

	size_t Shader::createMaterialBind(size_t offset, size_t indexTask)
	{
		for (size_t index = 0; index < _bindMaterial.size(); ++index)
		{
			auto &material = _bindMaterial[index];
			if (material.isUse == false)
			{
				setMaterialBinding(material, indexTask, offset);
				return (index);
			}
		}
		MaterialBindTask mb;
		setMaterialBinding(mb, indexTask, offset);
		_bindMaterial.push_back(mb);
		return (_bindMaterial.size() - 1);
	}

	Shader &Shader::unbindMaterial(Key<Uniform> const &key)
	{
		size_t binding;
		if ((binding = getUniformBindMaterial(key/*, "unbindMaterial"*/)) == -1)
			return (*this);
		_bindMaterial[binding].isUse = false;
		return (*this);
	}

	Shader &Shader::bindingTransformation(Key<Uniform> const &key)
	{
		_bindTransformation = key;
		return (*this);
	}

	GLuint Shader::addUnitProgByFile(std::string const &path, GLenum type)
	{
		GLuint shaderId;
		std::ifstream file(path.c_str(), std::ios_base::binary);
		GLchar *content;
		GLint fileSize;

		if (file.fail())
			assert(0);		
		file.seekg(0, file.end);
		fileSize = static_cast<GLint>(file.tellg()) + 1;
		file.seekg(0, file.beg);
		content = new GLchar[fileSize];
		file.read(content, fileSize - 1);
		content[fileSize - 1] = 0;
		shaderId = glCreateShader(type);
		glShaderSource(shaderId, 1, const_cast<const GLchar**>(&content), const_cast<const GLint*>(&fileSize));
		if (compileShader(shaderId, path) == false)
			assert(0);
		delete[] content;
		return (shaderId);
	}

	GLuint Shader::addUnitProg(char const *source, GLenum type)
	{
		GLuint shaderId;
		shaderId = glCreateShader(type);
		glShaderSource(shaderId, 1, (const GLchar**)(&source), NULL);
		if (compileShader(shaderId, "") == false)
			assert(0);
		return (shaderId);
	}

	bool Shader::compileShader(GLuint shaderId, std::string const &file)
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
			assert(0);
		}
		return (true);
	}
}