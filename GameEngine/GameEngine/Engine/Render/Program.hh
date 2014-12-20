#pragma once

# include <memory>
# include <Render/UnitProg.hh>
# include <Render/ProgramResources/IProgramResources.hh>
# include <Render/Key.hh>
# include <Render/GeometryManagement/Attribute.hh>
# include <Render/GeometryManagement/GraphicalMemory.hh>
# include <Render/ProgramResources/Factory/ProgramResourcesFactory.hh>

class Program
{
public:
	Program(std::vector<std::shared_ptr<UnitProg>> const &units);
	Program(Program const &copy) = delete;
	Program(Program &&move);
	~Program();
	Program &operator=(Program const &u) = delete;

public:
	GLuint id() const;
	Key<ProgramResource> &get_key(std::string const &name);
	IProgramResources *get_resource(Key<ProgramResource> const &key);
	IProgramResources *get_resource(std::string const &name);
	bool has_resource(Key<ProgramResource> const &key);
	Program const &use() const;
	Program const &print_resources() const;

private:
	void _create();
	void _get_resources();
	void _get_resource(size_t index, GLenum resource, std::string const & buffer);
	void _destroy();

private:
	std::vector<std::unique_ptr<IProgramResources>> _programResources;
	std::vector<std::shared_ptr<UnitProg>> _unitsProg;
	ProgramResourcesFactory _resources_factory;
	GLuint _id;
};