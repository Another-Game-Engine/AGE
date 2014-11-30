#pragma once

# include <memory>
# include <vector>
#include <Utils/OpenGL.hh>
#include <Utils/Dependency.hpp>
#include <Render/Key.hh>
#include <Render/Program.hh>

class UnitProg;

class RenderManager : public Dependency<RenderManager>
{
public:
	Key<UnitProg> addUnitProgram(std::string const &name, GLenum mode);
	Key<UnitProg> addUnitProgram(std::string &&name, GLenum mode);
	Key<Program> addProgram(std::vector<Key<UnitProg>> const &u1);
	template <typename resource_t> Key<ProgramResource> addProgramResource(Key<Program> const &program, std::string const &flag);
	template <typename resource_t> Key<ProgramResource> addProgramResource(Key<Program> const &program, std::string &&flag);
	template <typename resource_t> RenderManager &setProgramResource(Key<Program> const &program, Key<ProgramResource> const &resource, typename resource_t::type const &value);
	template <typename resource_t> RenderManager &setProgramResource(Key<Program> const &program, Key<ProgramResource> const &resource, typename resource_t::type &&value);

private:
	std::vector<Program> _programs;
	std::vector<std::shared_ptr<UnitProg>> _unitProgs;
};

template <typename resource_t>
inline Key<ProgramResource> RenderManager::addProgramResource(Key<Program> const &program, std::string const &name)
{
	auto &tmp_program = _programs[program.getId()];
	return (tmp_program.add<resource_t>(resource_t(resource_t::type(), tmp_program, std::move(name))));
}

template <typename resource_t>
inline Key<ProgramResource> RenderManager::addProgramResource(Key<Program> const &program, std::string &&name)
{
	auto &tmp_program = _programs[program.getId()];
	return (tmp_program.add<resource_t>(resource_t(resource_t::type(), tmp_program, std::move(name))));
}

template <typename resource_t>
inline RenderManager &RenderManager::setProgramResource(Key<Program> const &program, Key<ProgramResource> const &resource, typename resource_t::type const &value)
{
	auto &tmp_program = _programs[program.getId()];
	tmp_program.set<resource_t>(resource, value);
	return (*this);
}

template <typename resource_t>
inline RenderManager &RenderManager::setProgramResource(Key<Program> const &program, Key<ProgramResource> const &resource, typename resource_t::type &&value)
{
	auto &tmp_program = _programs[program.getId()];
	tmp_program.set<resource_t>(resource, std::move(value));
	return (*this);
}