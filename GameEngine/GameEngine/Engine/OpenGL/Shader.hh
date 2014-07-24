#pragma once

# include <Utils/OpenGL.hh>
# include <iostream>
# include <Utils/Containers/Vector.hpp>
# include <map>
# include <OpenGL/Key.hh>
# include <glm/glm.hpp>
# include <OpenGL/OpenGLTask.hh>
# include <OpenGL/Material.hh>

namespace gl
{

	class ShaderResource;
	class Texture;
	class UniformBlock;

	struct Sampler{};
	struct Uniform{};
	struct InterfaceBlock{};

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
		void updateMemory();

		template <typename TYPE>
		Shader bindingMaterial(Key<Uniform> const &key);
		Shader unbindMaterial(Key<Uniform> const &key);

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

		std::vector<MaterialBind> _bind;
		AGE::Vector<Task> _tasks;

		std::map<Key<Uniform>, size_t> _uniforms;
		std::map<Key<Sampler>, size_t> _samplers;
		std::map<Key<InterfaceBlock>, size_t> _interfaceBlock;

		// some private function usefull for internal functionement
		bool compileShader(GLuint shaderId, std::string const &file) const;
		bool linkProgram() const;
		GLuint addShader(std::string const &path, GLenum type);
		Task *getUniform(Key<Uniform> const &key, std::string const &msg);
		Task *getSampler(Key<Sampler> const &key, std::string const &msg);
		Task *getInterfaceBlock(Key<InterfaceBlock> const &key, std::string const &msg);
		GLuint getUniformLocation(char const *flag);
		GLuint getUniformBlockLocation(char const *flag);
		void createUniformTask(Task &task, std::string const &flag);
		void createSamplerTask(Task &task, std::string const &flag);
		void createUniformBlockTask(Task &task, std::string const &flag, UniformBlock const &ubo);
		void setSamplerTask(Task &task, Texture const &texture);
		template <typename TYPE> void setUniformTask(Task &task, void(*func)(void **), void *data);
		void setUniformBlockTask(Task &task, UniformBlock const &ubo);
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
		task.update = true;
		if (task.sizeParams[1] != sizeof(TYPE))
			DEBUG_MESSAGE("Warning", "Shader - setUniformTask", "size of setting different of dest", );
		memcpy(task.params[1], data, sizeof(TYPE));
	}

	template <typename TYPE>
	Shader Shader::bindingMaterial(Key<Uniform> const &key)
	{
		auto &element = _uniforms.find(key);
		if (element == _uniforms.end())
			DEBUG_MESSAGE("Warning", "Shader - bindingMaterial", "key not found in uniform", *this);
		size_t const index = element->second;
		Task const &task = _tasks[index];
		if (task.sizeParams[task.indexToTarget] != TYPE::size)
			DEBUG_MESSAGE("Warning", "Shader - bindingMaterial", "The size of material is not adapt with the uniform target", *this);
		for (size_t index = 0; index < _bind.size(); ++index)
		{
			if (_bind[index].isUse == false)
			{
				_bind[index].indexTask = index;
				_bind[index].offsetMaterial = TYPE::offset;
				_bind.isUse = true;
				_bindUniform[key] = index;
				return (*this);
			}
		}
		MaterialBind mb;
		mb.indexTask = index;
		mb.isUse = true;
		mb.offsetMaterial = TYPE::offset;
		_bind.push_back(mb);
		_bindUniform[key] = _bind.size() - 1;
		return (*this);
	}
}
