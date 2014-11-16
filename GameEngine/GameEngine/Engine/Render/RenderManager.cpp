#include <Render/RenderManager.hh>

Key<UnitProg> RenderManager::addUnitProgram(std::string const &name, GLenum mode)
{
	_unitProgs.emplace_back(std::make_shared<UnitProg>(name, mode));
	return (Key<UnitProg>::createKeyWithId(_unitProgs.size() - 1));
}

Key<UnitProg> RenderManager::addUnitProgram(std::string &&name, GLenum mode)
{
	_unitProgs.emplace_back(std::make_shared<UnitProg>(std::move(name), mode));
	return (Key<UnitProg>::createKeyWithId(_unitProgs.size() - 1));
}

Key<Program> RenderManager::addProgram(std::vector<Key<UnitProg>> const &units)
{
	std::vector<std::shared_ptr<UnitProg>> tmp_units;
	for (auto element : units)
		tmp_units.emplace_back(_unitProgs[element.getId()]);
	_programs.emplace_back(tmp_units);
	return (Key<Program>::createKeyWithId(_programs.size() - 1));
}