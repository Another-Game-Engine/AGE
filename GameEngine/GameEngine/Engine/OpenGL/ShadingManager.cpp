#include <OpenGL/ShadingManager.hh>
#include <OpenGL/Shader.hh>
#include <iostream>
#include <string>

# define DEBUG_MESSAGE(type, from, reason, return_type) \
{	std::cerr << std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "]." << std::endl; return return_type; }

namespace gl
{
	ShadingManager::ShadingManager()
	{
	}

	ShadingManager::~ShadingManager()
	{
	}

	Key<Shader> ShadingManager::addShader(std::string const &compute)
	{
		Key<Shader> key;
		Shader shader(compute);

		if (!shader.isValid())
			DEBUG_MESSAGE("Warning", "ShadingManager-addShader(string const &compute)", "compute invalid", Key<Shader>(KEY_DESTROY))
		_shaders[key] = shader;
		return (key);
	}

	Key<Shader> ShadingManager::addShader(std::string const &vertex, std::string const &frag)
	{
		Key<Shader> key;
		Shader shader(vertex, frag);

		if (!shader.isValid())
			DEBUG_MESSAGE("Warning", "ShadingManager-addShader(string const &frag, string const &vertex)", "frag and vertex invalid", Key<Shader>(KEY_DESTROY))
		_shaders[key] = shader;
		return (key);
	}

	Key<Shader> ShadingManager::addShader(std::string const &geo, std::string const &vertex, std::string const &frag)
	{
		Key<Shader> key;
		Shader shader(vertex, frag, geo);

		if (!shader.isValid())
			DEBUG_MESSAGE("Warning", "ShadingManager-addShader(string const &frag, string const &vertex, string const &geo)", "frag, vertex or geo invalid", Key<Shader>(KEY_DESTROY))
		_shaders[key] = shader;
		return (key);
	}

	ShadingManager &ShadingManager::rmShader(Key<Shader> &shader)
	{
		if (!shader)
			return (*this);
		_shaders.erase(shader);
		shader.destroy();
		return (*this);
	}

	Key<Shader> ShadingManager::getShader(size_t target) const
	{
		if (target >= _shaders.size())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp-getShader(size_t target)", "the target is out of range", Key<Shader>(KEY_DESTROY));
		auto &element = _shaders.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &shader, std::string const &flag)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderUniform", "key aldreadt destroy", Key<Uniform>(KEY_DESTROY))
			auto &element = _shaders.find(shader);
		if (element != _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderUniform", "shader not find", Key<Uniform>(KEY_DESTROY))
		auto &s = element->second;
		return (s.addUniform(flag));
	}

	ShadingManager &ShadingManager::rmShaderUniform(Key<Shader> const &shader, Key<Uniform> &uniform)
	{
		if (!shader && !uniform)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - rmShaderUniform", "key already destroy", *this)
		auto &element = _shaders.find(shader);
		if (element != _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - rmShaderUniform", "shader not find", *this)
		auto &s = element->second;
		s.rmUniform(uniform);
		return (*this);
	}

	Key<Uniform> ShadingManager::getShaderUniform(Key<Shader> const &shader, size_t target)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - getShaderUniform", "key destroy", Key<Uniform>(KEY_DESTROY))
		auto &element = _shaders.find(shader);
		if (element != _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - getShaderUniform", "shader not find", Key<Uniform>(KEY_DESTROY))
		auto &s = element->second; 
		return (s.getUniform(target));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::mat4 const &value)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderUniform", "key aldreadt destroy", Key<Uniform>(KEY_DESTROY))
			auto &element = _shaders.find(shader);
		if (element != _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - addShaderUniform", "shader not find", Key<Uniform>(KEY_DESTROY))
			auto &s = element->second;
		return (s.addUniform(flag, value));
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat4 const &mat4)
	{
		if (!shader)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderUniform(mat4)", "key destroy", *this)
			auto &element = _shaders.find(shader);
		if (element != _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - setShaderUniform", "shader not find", *this)
			auto &s = element->second;
		s.setUniform(key, mat4);
		return (*this);
	}
}