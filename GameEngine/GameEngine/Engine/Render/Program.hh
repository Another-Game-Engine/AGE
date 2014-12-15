#pragma once

# include <memory>
# include <Render/UnitProg.hh>
# include <Render/ProgramResources/IProgramResources.hh>
# include <Render/Key.hh>
# include <Render/GeometryManagement/Attribute.hh>
# include <Render/GeometryManagement/GraphicalMemory.hh>
# include <Render/ProgramResources/ProgramResourcesFactory.hh>

class Program
{
public:
	Program(std::vector<std::shared_ptr<UnitProg>> const &units);
	Program(Program const &copy) = delete;
	Program(Program &&move);
	~Program();
	Program &operator=(Program const &u) = delete;

public:
	GLuint getId() const;
	Key<ProgramResource> &addResource(std::string const &name);
	IProgramResources &getResource(Key<ProgramResource> const &key);
	bool has(Key<ProgramResource> const &key);
	Program const &use() const;

private:
	void _create();
	void _getProgramResources();
	void _destroy();

private:
	std::vector<std::unique_ptr<IProgramResources>> _programResources;
	std::vector<std::shared_ptr<UnitProg>> _unitsProg;
	std::unique_ptr<GraphicalMemory> _graphicalMemory;
	ProgramResourcesFactory _resources_factory;
	GLuint _id;
};