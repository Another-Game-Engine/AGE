#pragma once

# include <Utils/OpenGL.hh>
# include <vector>
# include <utility>
# include <functional>
# include <memory>
# include <Render/ProgramResources/IProgramResources.hh>

class Program;

class ProgramResourcesFactory
{
public:
	ProgramResourcesFactory(Program const &parent);
	ProgramResourcesFactory(ProgramResourcesFactory const &copy) = delete;
	ProgramResourcesFactory &operator=(ProgramResourcesFactory const &p) = delete;

public:
	std::unique_ptr<IProgramResources> build(GLenum mode, std::string &&name);

private:
	Program const &_parent;
	std::vector<std::pair<GLenum, std::function<std::unique_ptr<IProgramResources>(std::string &&name)>>> _blue_prints;
};