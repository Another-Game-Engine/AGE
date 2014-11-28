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
	Program(std::vector<std::shared_ptr<UnitProg>> const &units);
	Program(Program const &copy) = delete;
	Program(Program &&move);
	Program &operator=(Program const &u) = delete;
	Program &operator=(Program &&u);

public:
	Program &setAttribute(std::vector<Attribute> const &attibutes);
	Program &setAttribute(std::vector<Attribute> &&attibutes);
	GLuint getId() const;
	template <typename resource_t> Key<ResourceProgram> add(resource_t const &value);
	template <typename resource_t> Key<ResourceProgram> add(resource_t &&value);
	template <typename resource_t> Program &set(Key<ResourceProgram> const &key, typename resource_t::type const &value);
	template <typename resource_t> Program &set(Key<ResourceProgram> const &key, typename resource_t::type &&value);
	template <typename resource_t> Program &has(Key<ResourceProgram> const &key);
	Program &update();
	Program const &use() const;

private:
	void create();
	void destroy();

private:
	std::vector<std::unique_ptr<IProgramResource>> _resourcesProgram;
	std::vector<std::shared_ptr<UnitProg>> _unitsProg;
	GraphicalMemory _graphicalMemory;
	GLuint _id;
};

template <typename resource_t> 
Key<ResourceProgram> Program::add(resource_t const &resource)
{
	_resourcesProgram.emplace_back(std::make_unique<resource_t>(resource));
	return (Key<ResourceProgram>::createKey(_resourcesProgram.size() - 1));
}

template <typename resource_t> 
Key<ResourceProgram> Program::add(resource_t &&resource)
{
	_resourcesProgram.emplace_back(std::make_unique<resource_t>(std::move(resource)));
	return (Key<ResourceProgram>::createKey(_resourcesProgram.size() - 1));
}


template <typename resource_t>
Program & Program::set(Key<ResourceProgram> const &key, typename resource_t::type const &value)
{
	*static_cast<resource_t *>(_resourcesProgram[key.getId()].get()) = value;
	return (*this);
}

template <typename resource_t>
Program & Program::set(Key<ResourceProgram> const &key, typename resource_t::type &&value)
{
	*static_cast<resource_t *>(_resourcesProgram[key.getId()].get()) = std::move(value);
	return (*this);
}

template <typename resource_t>
Program & Program::has(Key<ResourceProgram> const &key)
{
	return (_resourceProgram.size() > key.g	etId());
}
