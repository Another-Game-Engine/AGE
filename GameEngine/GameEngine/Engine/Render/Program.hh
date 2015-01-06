#pragma once

# include <memory>
# include <Render/UnitProg.hh>
# include <Render/ProgramResources/IProgramResources.hh>
# include <Render/Key.hh>
# include <Render/ProgramResources/Factory/ProgramResourcesFactory.hh>

class Attribute;

class Program
{
public:
	Program(std::string &&name, std::vector<std::shared_ptr<UnitProg>> const &units);
	Program(Program const &copy) = delete;
	Program(Program &&move);
	~Program();
	Program &operator=(Program const &u) = delete;
	bool operator==(Program const &p) const;
	bool operator!=(Program const &p) const;

public:
	std::string const &name() const;
	GLuint id() const;
	Key<ProgramResource> &get_key(std::string const &name);
	template <typename type_t> std::shared_ptr<type_t> const &get_resource(Key<ProgramResource> const &key);
	template <typename type_t> std::shared_ptr<type_t> const &get_resource(std::string const &name);
	bool has_resource(Key<ProgramResource> const &key);
	Program const &use() const;
	Program &update();
	Program const &print_resources() const;
	size_t nbr_resources() const;
	bool coherent_attribute(std::vector<GLenum> const &p) const;

private:
	void _get_resources();
	void _get_resource(size_t index, GLenum resource, std::string const & buffer);

private:
	std::vector<std::shared_ptr<IProgramResources>> _program_resources;
	std::vector<std::shared_ptr<UnitProg>> _unitsProg;
	ProgramResourcesFactory _resources_factory;
	GLuint _id;
	std::string const _name;
};

template <typename type_t>
std::shared_ptr<type_t> const &Program::get_resource(std::string const &name)
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
std::shared_ptr<type_t> const & Program::get_resource(Key<ProgramResource> const &key)
{
	if (key) {
		return (nullptr);
	}
	return (_program_resources[key.getId()].get());
}