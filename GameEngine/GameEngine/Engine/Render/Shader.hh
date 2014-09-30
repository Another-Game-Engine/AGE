#pragma once

# include <Utils/OpenGL.hh>
# include <iostream>
# include <Utils/Containers/Vector.hpp>
# include <map>
# include <Render/Key.hh>
# include <glm/glm.hpp>
# include <Render/Task.hh>
# include <Render/Material.hh>
# include <Render/Storage.hh>
# include <Render/UniformBlock.hh>

namespace gl
{
	struct Sampler{};
	struct Uniform{};
	struct InterfaceBlock{};
	struct Output{};

	//!\file Shader.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class Shader
	//!\brief Handle Opengl Shader
	class Shader
	{
	public:
		Shader(Shader const &) = delete;
		Shader &operator=(Shader const &) = delete;
		~Shader(void);
		
		static Shader *createComputeShader(std::string const &name);
		static Shader *createShader(std::string const &v, std::string const &f);
		static Shader *createShader(std::string const &v, std::string const &f, std::string const &g);
		static Shader *createPreShaderQuad();

		void use() const;
		GLuint getId() const;

		// handling uniform management
		Key<Uniform> addUniform(std::string const &flag);
		Key<Uniform> addUniform(std::string const &flag, glm::mat4 const &value);
		Key<Uniform> addUniform(std::string const &flag, glm::mat3 const &value);
		Key<Uniform> addUniform(std::string const &flag, glm::vec4 const &value);
		Key<Uniform> addUniform(std::string const &flag, glm::vec3 const &value);
		Key<Uniform> addUniform(std::string const &flag, float value);
		Key<Uniform> getUniform(size_t index) const;
		Shader &setUniform(Key<Uniform> const &key, glm::mat4 const &mat4);
		Shader &setUniform(Key<Uniform> const &key, glm::mat3 const &mat3);
		Shader &setUniform(Key<Uniform> const &key, glm::vec4 const &vec4);
		Shader &setUniform(Key<Uniform> const &key, glm::vec3 const &vec4);
		Shader &setUniform(Key<Uniform> const &key, float v);

		// sampler 
		Key<Sampler> addSampler(std::string const &flag);
		Key<Sampler> getSampler(size_t index) const;
		Shader &setSampler(Key<Sampler> const &key, Texture const &bind);
		bool hasSampler(Key<Sampler> const &key);

		// InterfaceBlock
		Key<InterfaceBlock> addInterfaceBlock(std::string const &flag, UniformBlock &uniformblock);
		Key<InterfaceBlock> getInterfaceBlock(size_t index) const;
		Shader &setInterfaceBlock(Key<InterfaceBlock> const &key, UniformBlock &uniformBlock);
		Shader &introspection(Key<InterfaceBlock> const &key, UniformBlock &u);

		// update memory
		Shader &setMaterial(Material const &materia);
		Shader &update(glm::mat4 const &transform);
		Shader &update();

		Shader &bindingTransformation(Key<Uniform> const &key);
		template <typename TYPE> Shader &bindingMaterial(Key<Uniform> const &key);
		template <typename TYPE> Shader &bindingMaterial(Key<Sampler> const &key);
		Shader &unbindMaterial(Key<Uniform> const &key);

	private:
		Shader();

		GLuint *_unitProgId;
		GLuint	_progId;
		uint8_t _nbrUnitProgId;

		// binding for object in shader
		Key<Uniform> _bindTransformation;
		std::vector<MaterialBindTask> _bindMaterial;
		
		// pool stack
		AGE::Vector<Task> _tasks;

		// map of task
		std::map<Key<Uniform>, size_t> _bindUniform;
		std::map<Key<Uniform>, size_t> _uniforms;
		std::map<Key<Sampler>, size_t> _samplers;
		std::map<Key<InterfaceBlock>, size_t> _interfaceBlock;

		/// some private function usefull for internal functionement
		// use to create the shader
		static bool compileShader(GLuint shaderId, std::string const &file);
		static GLuint addUnitProgByFile(std::string const &path, GLenum type);
		static GLuint addUnitProg(char const *source, GLenum type);
		bool createProgram();
		bool linkProgram() const;

		// search function
		Task *getUniform(Key<Uniform> const &key);
		size_t getIndexUniform(Key<Uniform> const &key);
		Task *getSampler(Key<Sampler> const &key);
		size_t getIndexSampler(Key<Sampler> const &key);
		Task *getInterfaceBlock(Key<InterfaceBlock> const &key);
		size_t getIndexInterfaceBlock(Key<InterfaceBlock> const &key);
		Task *getOutput(Key<Output> const &key);
		size_t getIndexOutput(Key<Output> const &key);

		GLuint getUniformLocation(char const *flag);
		GLuint getUniformBlockLocation(char const *flag);
		GLuint getSamplerLocation(char const *flag);
		size_t getUniformBindMaterial(Key<Uniform> const &key, std::string const &msg);

		// use to create/set task and binding
		size_t createMaterialBind(size_t offset, size_t indexTask);
		void createUniformTask(Task &task, std::string const &flag);
		void createSamplerTask(Task &task, std::string const &flag);
		void createUniformBlockTask(Task &task, std::string const &flag, UniformBlock &ubo);
		void setMaterialBinding(MaterialBindTask &bind, size_t index, size_t offset);
		template <typename TYPE> void setUniformTask(Task &task, void(*func)(void **), void *data);
		void setSamplerTask(Task &task, Texture const &texture);
		void setUniformBlockTask(Task &task, UniformBlock &ubo);
		void setTaskWithMaterial(MaterialBindTask const &bind, Material const &material);
		void setTransformationTask(glm::mat4 const &mat);
	};

	template <typename TYPE>
	void Shader::setUniformTask(Task &task, void(*func)(void **), void *data)
	{
		if (task.params[1] == NULL)
		{
			task.params[1] = new TYPE;
			task.sizeParams[1] = sizeof(TYPE);
			task.func = func;
		}
		if (task.sizeParams[1] != sizeof(TYPE))
			assert(0);
		if (memcmp(task.params[1], data, sizeof(TYPE)) != 0)
		{
			task.update = true;
			memcpy(task.params[1], data, sizeof(TYPE));
		}
	}

	template <typename TYPE>
	Shader &Shader::bindingMaterial(Key<Uniform> const &key)
	{
		size_t indexTask;
		if ((indexTask = getIndexUniform(key)) == -1)
			return (*this);
		Task const &task = _tasks[indexTask];
		if (task.sizeParams[task.indexToTarget] != TYPE::size)
			assert(0);
		_bindUniform[key] = createMaterialBind(TYPE::offset, indexTask);
		return (*this);
	}

	template <typename TYPE>
	Shader &Shader::bindingMaterial(Key<Sampler> const &key)
	{
		size_t indexTask;
		if ((indexTask = getIndexSampler(key)) == -1)
			return (*this);
		Task const &task = _tasks[indexTask];
		if (task.sizeParams[task.indexToTarget] != TYPE::size)
			assert(0);
		_samplers[key] = createMaterialBind(TYPE::offset, indexTask);
		return (*this);
	}

	inline void Shader::setSamplerTask(Task &task, Texture const &texture)
	{
		GLint id = texture.getId();
		GLenum type = texture.getType();
		*(GLenum *)task.params[1] = texture.getType();
		*(GLint *)task.params[2] = texture.getId();
		task.update = true;
	}

	inline void Shader::setUniformBlockTask(Task &task, UniformBlock &ubo)
	{
		GLuint bindingPoint = ubo.getBindingPoint();
		GLuint id = ubo.getBufferId();
		if (memcmp(task.params[2], &bindingPoint, sizeof(GLuint)) != 0)
		{
			*(GLuint *)task.params[2] = bindingPoint;
			task.update = true;
		}
		if (memcmp(task.params[3], &id, sizeof(GLuint)) != 0)
		{
			*(GLuint *)task.params[3] = ubo.getBufferId();
			task.update = true;
		}
		if (memcmp(task.params[4], &ubo, sizeof(UniformBlock const *)) != 0)
		{
			*((UniformBlock const **)task.params[4]) = &ubo;
			task.update = true;
		}
		ubo.introspection(*this, *((GLuint *)task.params[1]));
	}

	inline void Shader::setMaterialBinding(MaterialBindTask &bind, size_t index, size_t offset)
	{
		bind.indexTask = index;
		bind.isUse = true;
		bind.offsetMaterial = offset;
	}

	inline void Shader::setTaskWithMaterial(MaterialBindTask const &bind, Material const &material)
	{
		Task &task = _tasks[bind.indexTask];
		size_t sizeParam = task.sizeParams[task.indexToTarget];
		if (memcmp(task.params[task.indexToTarget], material.getData(bind.offsetMaterial), sizeParam) == 0)
			return;
		memcpy(task.params[task.indexToTarget], material.getData(bind.offsetMaterial), sizeParam);
		task.update = true;
	}
}
