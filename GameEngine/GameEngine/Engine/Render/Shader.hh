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
		Key<Uniform> addUniform(std::string const &flag);
		Key<Uniform> addUniform(std::string const &flag, glm::mat4 const &value);
		Key<Uniform> addUniform(std::string const &flag, glm::mat3 const &value);
		Key<Uniform> addUniform(std::string const &flag, glm::vec4 const &value);
		Key<Uniform> addUniform(std::string const &flag, glm::vec3 const &value);
		Key<Uniform> addUniform(std::string const &flag, float value);
		Shader &setUniform(Key<Uniform> const &key, glm::mat4 const &mat4);
		Shader &setUniform(Key<Uniform> const &key, glm::mat3 const &mat3);
		Shader &setUniform(Key<Uniform> const &key, glm::vec4 const &vec4);
		Shader &setUniform(Key<Uniform> const &key, glm::vec3 const &vec4);
		Shader &setUniform(Key<Uniform> const &key, float v);
		Key<Sampler> addSampler(std::string const &flag);
		Shader &setSampler(Key<Sampler> const &key, Texture const &bind);
		bool hasSampler(Key<Sampler> const &key);
		Key<InterfaceBlock> addInterfaceBlock(std::string const &flag, UniformBlock &uniformblock);
		Key<InterfaceBlock> getInterfaceBlock(size_t index) const;
		Shader &setInterfaceBlock(Key<InterfaceBlock> const &key, UniformBlock &uniformBlock);
		Shader &introspection(Key<InterfaceBlock> const &key, UniformBlock &u);
		Shader &setMaterial(Material const &materia);
		Shader &update(glm::mat4 const &transform);
		Shader &update();
		Shader &bindingTransformation(Key<Uniform> const &key);
		template <typename TYPE> Shader &bindingMaterial(Key<Uniform> const &key);
		template <typename TYPE> Shader &bindingMaterial(Key<Sampler> const &key);
		Shader &unbindMaterial(Key<Uniform> const &key);

	private:
		Shader();
		static bool compileShader(GLuint shaderId, std::string const &file);
		static GLuint addUnitProgByFile(std::string const &path, GLenum type);
		static GLuint addUnitProg(char const *source, GLenum type);
		bool createProgram();
		bool linkProgram() const;
		Task *getUniform(Key<Uniform> const &key);
		Task *getSampler(Key<Sampler> const &key);
		Task *getInterfaceBlock(Key<InterfaceBlock> const &key);
		Task *getOutput(Key<Output> const &key);
		size_t getIndexOutput(Key<Output> const &key);
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
		GLuint getUniformLocation(char const *flag);
		GLuint getUniformBlockLocation(char const *flag);
		GLuint getSamplerLocation(char const *flag);
		size_t getUniformBindMaterial(Key<Uniform> const &key, std::string const &msg);
	
	private:
		size_t _shaderNumber;
		GLuint *_unitProgId;
		GLuint	_progId;
		uint8_t _nbrUnitProgId;
		Key<Uniform> _bindTransformation;
		AGE::Vector<MaterialBindTask> _bindMaterial;
		AGE::Vector<Task> _tasks;
		AGE::Vector<size_t> _bindSampler;
		AGE::Vector<size_t> _bindUniform;
		AGE::Vector<size_t> _uniforms;
		AGE::Vector<size_t> _samplers;
		AGE::Vector<size_t> _interfaceBlock;
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
		Task const &task = *getUniform(key);
		assert(task.sizeParams[task.indexToTarget] == TYPE::size);
		if (_bindUniform.size() <= key.getId())
			_bindUniform.resize(key.getId() + 1, -1);
		_bindUniform[key.getId()] = createMaterialBind(TYPE::offset, _uniforms[key.getId()]);
		return (*this);
	}

	template <typename TYPE>
	Shader &Shader::bindingMaterial(Key<Sampler> const &key)
	{
		Task const &task = *getSampler(key);
		if (_bindSampler.size() <= key.getId())
			_bindSampler.resize(key.getId() + 1, -1);
		_bindSampler[key.getId()] = createMaterialBind(TYPE::offset, _samplers[key.getId()]);
		return (*this);
	}

	inline void Shader::setSamplerTask(Task &task, Texture const &texture)
	{
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
		if (task.type == TypeTask::UniformTask)
		{
			if (memcmp(task.params[task.indexToTarget], material.getData(bind.offsetMaterial), sizeParam) == 0)
				return;
		}
		memcpy(task.params[task.indexToTarget], material.getData(bind.offsetMaterial), sizeParam);
		task.update = true;
	}
}
