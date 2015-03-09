#pragma once

# include <memory>
# include <Render/UnitProg.hh>
# include <Render/ProgramResources/IProgramResources.hh>
# include <Render/Key.hh>
# include <Render/ProgramResources/Factory/ProgramResourcesFactory.hh>

namespace AGE
{
	class Attribute;
	class BufferPrograms;

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
		template <typename type_t> std::shared_ptr<type_t> get_resource(Key<ProgramResource> const &key);
		template <typename type_t> std::shared_ptr<type_t> get_resource(std::string const &name);
		std::shared_ptr<IProgramResources> get_resource_interface(std::string const &name);
		bool has_resource(Key<ProgramResource> const &key);
		Program const &use() const;
		Program &update();
		Program const &print_resources() const;
		size_t nbr_resources() const;
		Program &set_attributes(BufferPrograms const &buffers);
		bool coherent_attributes(std::vector<std::pair<GLenum, std::string>> const &coherent);
		bool compile();
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
	std::shared_ptr<type_t> Program::get_resource(std::string const &name)
	{
		for (size_t index = 0; index < _program_resources.size(); ++index)
		{
			if (name == _program_resources[index]->name())
			{
				auto &resource = _program_resources[index];
				return (resource->safe(sizeof(type_t::type_t)) ? std::static_pointer_cast<type_t>(resource) : nullptr);
			}
		}
		return (nullptr);
	}

	template <typename type_t>
	std::shared_ptr<type_t> Program::get_resource(Key<ProgramResource> const &key)
	{
		if (key) {
			return (nullptr);
		}
		return (_program_resources[key.getId()]->safe(sizeof(type_t::type_t)) ? _program_resources[key.getId()].get() : nullptr);
	}
}