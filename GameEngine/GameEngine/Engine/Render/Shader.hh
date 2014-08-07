#pragma once

# include <Utils/OpenGL.hh>
# include <iostream>
# include <Utils/Containers/Vector.hpp>
# include <map>
# include <Render/Key.hh>
# include <glm/glm.hpp>
# include <Render/OpenGLTask.hh>
# include <Render/Material.hh>
#include <Render/Storage.hh>
#include <Render/UniformBlock.hh>

namespace gl
{

	class ShaderResource;

	struct Sampler{};
	struct Uniform{};
	struct InterfaceBlock{};
	struct Output{};

	struct MaterialBind
	{
		size_t indexTask;
		size_t offsetMaterial;
		bool isUse;
	};

	//!\file Shader.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class Shader
	//!\brief Handle Opengl Shader
	class Shader
	{
	public:
		Shader();
		Shader(std::string const &compute);
		Shader(std::string const &vertex, std::string const &fragment);
		Shader(std::string const &vertex, std::string const &fragment, std::string const &geometry);
		Shader(Shader const &Shader);
		Shader &operator=(Shader const &shader);
		~Shader(void);

		void use() const;
		bool isValid() const;
		GLuint getId() const;

		std::string const &getVertexName() const;
		std::string const &getFragName() const;
		std::string const &getGeoName() const;
		std::string const &getComputeName() const;

		// handling uniform management
		Key<Uniform> addUniform(std::string const &flag);
		Key<Uniform> addUniform(std::string const &flag, glm::mat4 const &value);
		Key<Uniform> addUniform(std::string const &flag, glm::mat3 const &value);
		Key<Uniform> addUniform(std::string const &flag, glm::vec4 const &value);
		Key<Uniform> addUniform(std::string const &flag, float value);
		Key<Uniform> getUniform(size_t index) const;
		Shader &setUniform(Key<Uniform> const &key, glm::mat4 const &mat4);
		Shader &setUniform(Key<Uniform> const &key, glm::mat3 const &mat3);
		Shader &setUniform(Key<Uniform> const &key, glm::vec4 const &vec4);
		Shader &setUniform(Key<Uniform> const &key, float v);

		// sampler 
		Key<Sampler> addSampler(std::string const &flag);
		Key<Sampler> getSampler(size_t index) const;
		Shader &setSampler(Key<Sampler> const &key, Texture const &bind);
		
		// InterfaceBlock
		Key<InterfaceBlock> addInterfaceBlock(std::string const &flag, UniformBlock const &uniformblock);
		Key<InterfaceBlock> getInterfaceBlock(size_t index) const;
		Shader setInterfaceBlock(Key<InterfaceBlock> const &key, UniformBlock const &uniformblock);

		// update memory
		void preDraw(Material const &material, glm::mat4 const &transform);

		Shader &bindingTransformation(Key<Uniform> const &key);
		template <typename TYPE> Shader &bindingMaterial(Key<Uniform> const &key);
		template <typename TYPE> Shader &bindingMaterial(Key<Sampler> const &key);
		Shader &unbindMaterial(Key<Uniform> const &key);

	private:
		std::string _vertexName;
		std::string _fragName;
		std::string _geometryName;
		std::string _computeName;
		GLuint	_progId;
		GLuint	_vertexId;
		GLuint	_fragId;
		GLuint	_geometryId;
		GLuint	_computeId;

		Key<Uniform> _bindTransformation;
		std::vector<MaterialBind> _bind;
		AGE::Vector<Task> _tasks;

		std::map<Key<Uniform>, size_t> _bindUniform;
		std::map<Key<Uniform>, size_t> _uniforms;
		std::map<Key<Sampler>, size_t> _samplers;
		std::map<Key<InterfaceBlock>, size_t> _interfaceBlock;

		// some private function usefull for internal functionement
		bool compileShader(GLuint shaderId, std::string const &file) const;
		bool linkProgram() const;
		GLuint addShader(std::string const &path, GLenum type);
		
		Task *getUniform(Key<Uniform> const &key, std::string const &msg);
		size_t getIndexUniform(Key<Uniform> const &key, std::string const &msg);
		Task *getSampler(Key<Sampler> const &key, std::string const &msg);
		size_t getIndexSampler(Key<Sampler> const &key, std::string const &msg);
		Task *getInterfaceBlock(Key<InterfaceBlock> const &key, std::string const &msg);
		size_t getIndexInterfaceBlock(Key<InterfaceBlock> const &key, std::string const &msg);
		Task *getOutput(Key<Output> const &key, std::string const &msg);
		size_t getIndexOutput(Key<Output> const &key, std::string const &msg);


		GLuint getUniformLocation(char const *flag);
		GLuint getUniformBlockLocation(char const *flag);
		size_t getUniformBindMaterial(Key<Uniform> const &key, std::string const &msg);

		size_t createMaterialBind(size_t offset, size_t indexTask);
		void createUniformTask(Task &task, std::string const &flag);
		void createSamplerTask(Task &task, std::string const &flag);
		void createUniformBlockTask(Task &task, std::string const &flag, UniformBlock const &ubo);
		void setMaterialBinding(MaterialBind &bind, size_t index, size_t offset);
		template <typename TYPE> void setUniformTask(Task &task, void(*func)(void **), void *data);
		void setSamplerTask(Task &task, Texture const &texture);
		void setUniformBlockTask(Task &task, UniformBlock const &ubo);
		void setTaskWithMaterial(MaterialBind const &bind, Material const &material);
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
			DEBUG_MESSAGE("Warning", "Shader - setUniformTask", "size of setting different of dest", );
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
		if ((indexTask = getIndexUniform(key, "bindingMaterial")) == -1)
			return (*this);
		Task const &task = _tasks[indexTask];
		if (task.sizeParams[task.indexToTarget] != TYPE::size)
			DEBUG_MESSAGE("Warning", "Shader - bindingMaterial", "The size of material is not adapt with the uniform target", *this);
		_bindUniform[key] = createMaterialBind(TYPE::offset, indexTask);
		return (*this);
	}

	template <typename TYPE>
	Shader &Shader::bindingMaterial(Key<Sampler> const &key)
	{
		size_t indexTask;
		if ((indexTask = getIndexSampler(key, "bindingMaterial")) == -1)
			return (*this);
		Task const &task = _tasks[indexTask];
		if (task.sizeParams[task.indexToTarget] != TYPE::size)
			DEBUG_MESSAGE("Warning", "Shader - bindingMaterial", "The size of material is not adapt with the uniform target", *this);
		_bindUniform[key] = createMaterialBind(TYPE::offset, indexTask);
		return (*this);
	}

	inline void Shader::setSamplerTask(Task &task, Texture const &texture)
	{
		GLint id = texture.getId();
		GLenum type = texture.getType();
		if (memcmp(task.params[1], &type, sizeof(GLenum)) != 0)
		{
			*(GLenum *)task.params[1] = texture.getType();
			task.update = true;
		}
		if (memcmp(task.params[2], &id, sizeof(GLint)) != 0)
		{
			*(GLint *)task.params[2] = texture.getId();
			task.update = true;
		}
	}

	inline void Shader::setUniformBlockTask(Task &task, UniformBlock const &ubo)
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
	}

	inline void Shader::setMaterialBinding(MaterialBind &bind, size_t index, size_t offset)
	{
		bind.indexTask = index;
		bind.isUse = true;
		bind.offsetMaterial = offset;
	}

	inline void Shader::setTaskWithMaterial(MaterialBind const &bind, Material const &material)
	{
		Task &task = _tasks[bind.indexTask];
		size_t sizeParam = task.sizeParams[task.indexToTarget];
		if (memcmp(task.params[task.indexToTarget], material.getData(bind.offsetMaterial), sizeParam) == 0)
			return;
		memcpy(task.params[task.indexToTarget], material.getData(bind.offsetMaterial), sizeParam);
		task.update = true;
	}

}