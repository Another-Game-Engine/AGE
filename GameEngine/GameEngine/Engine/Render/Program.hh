#pragma once

# include <memory>
# include <Render/UnitProg.hh>
# include <Render/ProgramResources/IProgramResources.hh>
# include <Render/Key.hh>
# include <Render/GeometryManagement/Attribute.hh>
# include <Render/GeometryManagement/GraphicalMemory.hh>

class Program
{
public:
	Program(std::vector<std::shared_ptr<UnitProg>> const &units, std::vector<Attribute> const &attibutes);
	Program(Program const &copy) = delete;
	Program(Program &&move);
	Program &operator=(Program const &u) = delete;

public:
	GLuint getId() const;
	Key<ProgramResource> &addResource(std::unique_ptr<IProgramResources> const &value);
	std::unique_ptr<IProgramResources> const &getResource(Key<ProgramResource> const &key);
	bool has(Key<ProgramResource> const &key);
	Program &update();
	Program const &use() const;

private:
	void create();
	void destroy();

private:
	std::vector<std::unique_ptr<IProgramResources>> _resourcesProgram;
	std::vector<std::shared_ptr<UnitProg>> _unitsProg;
	GraphicalMemory _graphicalMemory;
	GLuint _id;
};