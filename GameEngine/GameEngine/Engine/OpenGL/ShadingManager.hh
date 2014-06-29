#pragma once

#include <Utils/OpenGL.hh>
#include <Utils/Dependency.hpp>
#include <OpenGL/Key.hh>
#include <OpenGL/MemoryGPU.hh>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <OpenGL/UniformBlock.hh>

namespace gl
{
	class Shader;
	struct Uniform;
	struct Sampler;
	struct InterfaceBlock;
	class Texture;
	class UniformBuffer;


	//!\file ShadingManager.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class ShadingManager
	//!\brief Handle the shading of the object
	class ShadingManager : public Dependency
	{
	public:
		struct Attach
		{
			Shader const *shaderAttach;
			UniformBuffer *buffer;
			//UniformBlock *uniformblock;
		};
	public:
		ShadingManager();
		~ShadingManager();

		// shader handling
		Key<Shader> addShader(std::string const &compute);
		Key<Shader> addShader(std::string const &vert, std::string const &frag);
		Key<Shader> addShader(std::string const &geometry, std::string const &vert, std::string const &frag);
		ShadingManager &rmShader(Key<Shader> &shader);
		Key<Shader> getShader(size_t index) const;
		ShadingManager &useShader(Key<Shader> const &s);
		// uniform
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::mat4 const &value);
		ShadingManager &rmShaderUniform(Key<Shader> const &shader, Key<Uniform> &uniform);
		Key<Uniform> getShaderUniform(Key<Shader> const &shader, size_t index);
		ShadingManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat4 const &mat4);
		ShadingManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::vec4 const &vec4);
		ShadingManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, float v);
		ShadingManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat3 const &mat3);
		// sampler
		Key<Sampler> addShaderSampler(Key<Shader> const &shader, std::string const &flag);
		ShadingManager &rmShaderSampler(Key<Shader> const &shader, Key<Sampler> &uniform);
		Key<Sampler> getShaderSampler(Key<Shader> const &shader, size_t index);
		ShadingManager &setShaderSampler(Key<Shader> const &shader, Key<Sampler> const &key, Texture const &texture);
		// Interface
		Key<InterfaceBlock> addShaderInterfaceBlock(Key<Shader> const &shader, std::string const &flag);
		Key<InterfaceBlock> addShaderInterfaceBlock(Key<Shader> const &shader, std::string const &flag, Key<UniformBlock> const &keyUniformBlock);
		ShadingManager &rmShaderInterfaceBlock(Key<Shader> const &shader, Key<InterfaceBlock> &key);
		Key<InterfaceBlock> getShaderInterfaceBlock(Key<Shader> const &shader, size_t index);
		ShadingManager &setShaderInterfaceBlock(Key<Shader> const &shader, Key<InterfaceBlock> const &key, Key<UniformBlock> const &keyUniformBlock);

		// uniform Block
		Key<UniformBlock> addUniformBlock(size_t nbrElement, size_t *sizeElement);
		ShadingManager &rmUniformBlock(Key<UniformBlock> &uniformBlock);
		Key<UniformBlock> getUniformBlock(size_t index);
		template <typename TYPE> ShadingManager &setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value);

		//pipeline handling
		//Key<Pipeline> addPipeline(size_t nbrShader, Key<Shader> *shaders);
		//ShadingManager &rmPipeline(Key<Pipeline> const &pipeline);
		//Key<Pipeline> const &getPipeline(size_t index) const;
	private:
		std::map<Key<Shader>, Shader> _shaders;
		std::map<Key<UniformBlock>, UniformBlock> _uniformBlock;
		//std::map<Key<Pipeline>, Pipeline> _pipelines;

	};

# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	std::cerr << std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "]." << std::endl; return return_type; }

	template <typename TYPE>
	ShadingManager &ShadingManager::setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "ShadingManager.hh - setUniformBlock()", "key is destroy", *this);
		auto &element = _uniformBlock.find(key);
		if (element == _uniformBlock.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.hh - setUniformBlock()", "the uniformBlock ask is not in list", *this);
		UniformBlock &uniformblock = element->second;
		uniformblock.set<TYPE>(index, value);
		return (*this);
	}

	template <typename TYPE1>
	inline void set_tab_sizetype(size_t *tab)
	{
		tab[0] = sizeof(TYPE1);
	}

	template <typename TYPE1, typename TYPE2>
	inline void set_tab_sizetype(size_t *tab)
	{
		tab[0] = sizeof(TYPE1); tab[1] = sizeof(TYPE2);
	}

	template <typename TYPE1, typename TYPE2, typename TYPE3>
	inline void set_tab_sizetype(size_t *tab)
	{
		tab[0] = sizeof(TYPE1); tab[1] = sizeof(TYPE2); tab[2] = sizeof(TYPE3);
	}

	template <typename TYPE1, typename TYPE2, typename TYPE3, typename TYPE4>
	inline void set_tab_sizetype(size_t *tab)
	{
		tab[0] = sizeof(TYPE1); tab[1] = sizeof(TYPE2); tab[2] = sizeof(TYPE3); tab[3] = sizeof(TYPE4);
	}

	template <typename TYPE1, typename TYPE2, typename TYPE3, typename TYPE4, typename TYPE5>
	inline void set_tab_sizetype(size_t *tab)
	{
		tab[0] = sizeof(TYPE1); tab[1] = sizeof(TYPE2); tab[2] = sizeof(TYPE3); tab[3] = sizeof(TYPE4); tab[4] = sizeof(TYPE5);
	}

	template <typename TYPE1, typename TYPE2, typename TYPE3, typename TYPE4, typename TYPE5, typename TYPE6>
	inline void set_tab_sizetype(size_t *tab)
	{
		tab[0] = sizeof(TYPE1); tab[1] = sizeof(TYPE2); tab[2] = sizeof(TYPE3); tab[3] = sizeof(TYPE4); tab[4] = sizeof(TYPE5); tab[5] = sizeof(TYPE6);
	}
}