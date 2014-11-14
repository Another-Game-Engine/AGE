#pragma once

# include <vector>
# include <Render/UnitProg.hh>
# include <Render/ResourceProgram.hh>
# include <Render/Key.hh>
# include <Render/Attribute.hh>



class VertexPool;
class IndexPool;

class Program
{
public:
	Program(UnitProg const &u);
	Program(UnitProg const &u1, UnitProg const &u2);
	Program(UnitProg const &u1, UnitProg const &u2, UnitProg const &u3);
	Program(UnitProg &&u);
	Program(UnitProg &&u1, UnitProg &&u2);
	Program(UnitProg &&u1, UnitProg &&u2, UnitProg &&u3);
	Program(Program const &copy);
	Program(Program &&move);
	Program &operator=(Program const &u);
	Program &operator=(Program &&u);

public:
	Program &setAttribute(std::vector<Attribute> const &attibutes);
	Program &setAttribute(std::vector<Attribute> &&attibutes);
	GLuint getId() const;
	template <typename resource_t> Key<ResourceProgram> add(std::string const &name, resource_t const &value);
	template <typename resource_t> Key<ResourceProgram> add(std::string &&name, resource_t &&value);
	template <typename resource_t> Program &set(Key<ResourceProgram> const &key, resource_t const &value);
	template <typename resource_t> Program &set(Key<ResourceProgram> const &key, resource_t &&value);
	template <typename resource_t> Program &has(Key<ResourceProgram> const &key);
	Program &update();

private:
	void create();
	void destroy();

private:
	std::vector<IResourceProgram *> _resourcesProgram;
	std::vector<UnitProg> _unitsProg;
	GLuint _id;
};

template <typename resource_t> 
Key<ResourceProgram> Program::add(std::string const &name, resource_t const &value)
{	
	_resourceProgram.emplace_back(value);
	return (Key<ResourceProgram>::createKeyWithId(_resourcesProgram.size() - 1));
}

template <typename resource_t> 
Key<ResourceProgram> Program::add(std::string &&name, resource_t &&value)
{
	_resourceProgram.emplace_back(std::move(value));
	return (Key<ResourceProgram>::createKeyWithId(_resourcesProgram.size() - 1));
}


template <typename resource_t>
Program & Program::set(Key<ResourceProgram> const &key, resource_t const &value)
{
	_resourceProgram[key.getId()] = value;
	return (*this);
}

template <typename resource_t>
Program & Program::set(Key<ResourceProgram> const &key, resource_t &&value)
{
	_resourceProgram[key.getId()] = std::move(value);
	return (*this);
}

template <typename resource_t>
Program & Program::has(Key<ResourceProgram> const &key)
{
	return (_resourceProgram.size() > key.getId());
}
