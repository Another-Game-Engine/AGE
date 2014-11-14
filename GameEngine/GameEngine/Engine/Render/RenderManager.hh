#pragma once

#include <Utils/OpenGL.hh>
#include <Utils/Dependency.hpp>
#include <Render/Key.hh>
#include <Render/Program.hh>

class UnitProg;

class RenderManager : public Dependency<RenderManager>
{
public:
	RenderManager();
	~RenderManager();

public:
	Key<UnitProg> addUnitProgram(std::string const &name, GLenum mode);
	Key<UnitProg> addUnitProgram(std::string &&name, GLenum mode);
	Key<Program> addProgram(Key<UnitProg> const &u1);
	Key<Program> addProgram(Key<UnitProg> const &u1, Key<UnitProg> const &u2);
	Key<Program> addProgram(Key<UnitProg> const &u1, Key<UnitProg> const &u2, Key<UnitProg> const &u3);
	Key<Program> addProgram(Key<UnitProg> &&u1);
	Key<Program> addProgram(Key<UnitProg> &&u1, Key<UnitProg> &&u2);
	Key<Program> addProgram(Key<UnitProg> &&u1, Key<UnitProg> &&u2, Key<UnitProg> &&u3);
	template <typename resource_t> Key<ResourceProgram> addResourceProgram(Key<Program> const &program, std::string const &flag);
	template <typename resource_t> Key<ResourceProgram> addResourceProgram(Key<Program> const &program, std::string &&flag);
	template <typename resource_t> RenderManager &setResourceProgram(Key<Program> const &program, Key<ResourceProgram> const &resource, resource_t const &value);

private:
	std::vector<Program> _programs;
	std::vector<UnitProg> _unitProgs;

private:
	Program *getProgram(Key<Program> const &key){ assert(!!key); return (&_programs[key.getId()]); }
	UnitProg *getUnitProg(Key<UnitProg> const &key) { assert(!!key); return (&_unitProgs[key.getId()]); }
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
	return (tmp_program->add(std::move(name), resource_t()));
}

template <typename resource_t>
inline RenderManager &RenderManager::setResourceProgram(Key<Program> const &program, Key<ResourceProgram> const &resource, resource_t const &value)
{
	auto tmp_program = getProgram(program);
	tmp_program->set(resource, value);
	return (*this);
}