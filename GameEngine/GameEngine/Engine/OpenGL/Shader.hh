#pragma once

# include <Utils/OpenGL.hh>
# include <iostream>
# include <vector>
# include <map>
# include <OpenGL/Key.hh>
# include <glm/glm.hpp>
# include <OpenGL/OpenGLTask.hh>

namespace gl
{

	class ShaderResource;
	class Texture;
	class UniformBlock;

	struct Sampler{};
	struct Uniform{};
	struct InterfaceBlock{};

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
		Shader &rmUniform(Key<Uniform> &key);
		Key<Uniform> getUniform(size_t index) const;
		Shader &setUniform(Key<Uniform> const &key, glm::mat4 const &mat4);
		Shader &setUniform(Key<Uniform> const &key, glm::mat3 const &mat3);
		Shader &setUniform(Key<Uniform> const &key, glm::vec4 const &vec4);
		Shader &setUniform(Key<Uniform> const &key, float v);

		// sampler 
		Key<Sampler> addSampler(std::string const &flag);
		Shader &rmSampler(Key<Sampler> &key);
		Key<Sampler> getSampler(size_t index) const;
		Shader &setSampler(Key<Sampler> const &key, Texture const &bind);
		
		// InterfaceBlock
		Key<InterfaceBlock> addInterfaceBlock(std::string const &flag, UniformBlock const &uniformblock);
		Shader &rmInterfaceBlock(Key<InterfaceBlock> &key);
		Key<InterfaceBlock> getInterfaceBlock(size_t index) const;
		Shader setInterfaceBlock(Key<InterfaceBlock> const &key, UniformBlock const &uniformblock);

		//
		void updateMemory();
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

		std::vector<Task> _tasks;
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
		if (task.sizeParams[1] != sizeof(TYPE))
			DEBUG_MESSAGE("Warning", "Shader - setUniformTask", "size of setting different of dest", );
		memcpy(task.params[1], data, sizeof(TYPE));
	}
}
