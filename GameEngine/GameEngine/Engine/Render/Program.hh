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
	template <typename type_t> type_t *get_resource(Key<ProgramResource> const &key);
	template <typename type_t> type_t *get_resource(std::string const &name);
	bool has_resource(Key<ProgramResource> const &key);
	Program const &use() const;
	Program const &print_resources() const;

private:
	void _get_resources();
	void _get_resource(size_t index, GLenum resource, std::string const & buffer);

private:
	std::vector<std::unique_ptr<IProgramResources>> _program_resources;
	std::vector<std::shared_ptr<UnitProg>> _unitsProg;
	ProgramResourcesFactory _resources_factory;
	GLuint _id;
};

template <typename type_t>
type_t * Program::get_resource(std::string const &name)
{
	for (size_t index = 0; index < _program_resources.size(); ++index) {
		if (name == _program_resources[index]->name()) {
			auto resource = _program_resources[index].get();
			return (resource->safe(sizeof(type_t::type_t)) ? static_cast<type_t *>(resource) : nullptr);
		}
	}
	return (nullptr);
}

template <typename type_t>
type_t * Program::get_resource(Key<ProgramResource> const &key)
{
	if (key) {
		return (nullptr);
	}
	return (_program_resources[key.getId()].get());
}