#pragma once

#include <Utils/OpenGL.hh>
#include <Utils/Dependency.hpp>
#include <Render/Key.hh>
#include <Render/MemoryGPU.hh>
#include <map>
#include <Utils/Containers/Vector.hpp>
#include <glm/glm.hpp>
#include <Render/UniformBlock.hh>
#include <Render/Material.hh>
#include <cassert>
#include <Render/Render.hh>
#include <Render/Program.hh>
#include <Render/MacroRenderManager.hh>
#include <Core/CullableObjects.hh>
#include <Render/LocationStorage.hh>
#include <tmq/message.hpp>
#include <Render/Pipeline.hh>

class RenderManager : public Dependency<RenderManager>
{
public:
	RenderManager();
	~RenderManager();

public:
	template <typename resource_t> Key<ResourceProgram> addResourceProgram(Key<Program> const &program, std::string const &flag);
	template <typename resource_t> Key<ResourceProgram> addResourceProgram(Key<Program> const &program, std::string &&flag);
	template <typename resource_t> RenderManager &setResourceProgram(Key<Program> const &program, Key<ResourceProgram> const &resource, resource_t const &value);

private:
	std::vector<Program> _programs;
	size_t _layout;

private:
	Program *getProgram(Key<Program> const &key){ assert(!!key); return (&_programs[key.getId()]); }
};

template <typename resource_t>
inline Key<ResourceProgram> RenderManager::addResourceProgram(Key<Program> const &program, std::string const &name)
{
	auto tmp_program = getProgram(program);
	return (tmp_program->add(name, resource_t()));
}

template <typename resource_t>
inline Key<ResourceProgram> RenderManager::addResourceProgram(Key<Program> const &program, std::string &&name)
{
	auto tmp_program = getProgram(program);
	return (tmp_program->add(std::move(name), resource_t());
}

template <typename resource_t>
inline RenderManager &RenderManager::setResourceProgram(Key<Program> const &program, Key<ResourceProgram> const &resource, resource_t const &value)
{
	auto tmp_program = getProgram(program);
	tmp_program->set(resource, value);
	return (*this);
}

template <typename TYPE>
RenderManager &RenderManager::setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value)
{
	UniformBlock *uniformBlock = getUniformBlock(key);
	uniformBlock->set<TYPE>(index, value);
	return (*this);
}

template <typename TYPE>
RenderManager &RenderManager::setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value, size_t indexTab)
{
	UniformBlock *uniformBlock = getUniformBlock(key));
	uniformBlock->set<TYPE>(index, value, indexTab);
	return (*this);
}