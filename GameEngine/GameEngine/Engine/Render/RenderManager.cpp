#include <Render/RenderManager.hh>

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

Key<UnitProg> RenderManager::addUnitProgram(std::string const &name, GLenum mode)
{
	_unitProgs.emplace_back(UnitProg(name, mode));
	return (Key<UnitProg>::createKeyWithId(_unitProgs.size() - 1));
}

Key<UnitProg> RenderManager::addUnitProgram(std::string &&name, GLenum mode)
{
	_unitProgs.emplace_back(std::move(name), mode);
	return (Key<UnitProg>::createKeyWithId(_unitProgs.size() - 1));
}

Key<Program> RenderManager::addProgram(Key<UnitProg> const &u1)
{
	_programs.emplace_back(u1);
	return (Key<Program>::createKeyWithId(_programs.size() - 1));
}

Key<Program> RenderManager::addProgram(Key<UnitProg> const &u1, Key<UnitProg> const &u2)
{
	_programs.emplace_back(u1, u2);
	return (Key<Program>::createKeyWithId(_programs.size() - 1));
}

Key<Program> RenderManager::addProgram(Key<UnitProg> const &u1, Key<UnitProg> const &u2, Key<UnitProg> const &u3)
{
	_programs.emplace_back(u1, u2, u3);
	return (Key<Program>::createKeyWithId(_programs.size() - 1));
}

Key<Program> RenderManager::addProgram(Key<UnitProg> &&u1)
{
	_programs.emplace_back(std::move(u1));
	return (Key<Program>::createKeyWithId(_programs.size() - 1));
}

Key<Program> RenderManager::addProgram(Key<UnitProg> &&u1, Key<UnitProg> &&u2)
{
	_programs.emplace_back(std::move(u1), std::move(u2));
	return (Key<Program>::createKeyWithId(_programs.size() - 1));
}

Key<Program> RenderManager::addProgram(Key<UnitProg> &&u1, Key<UnitProg> &&u2, Key<UnitProg> &&u3)
{
	_programs.emplace_back(std::move(u1), std::move(u2), std::move(u3));
	return (Key<Program>::createKeyWithId(_programs.size() - 1));
}