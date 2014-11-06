#pragma once

# include <Render/UnitProg.hh>
# include <Render/ResourceProgram.hh>
# include <Render/Key.hh>
# include <vector>
# include <array>
# include <memory>

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
	Program setAttribute(std::vector<std::string> const &attibutes);
	GLuint getId() const;
	bool hasGeometryShader() const;
	bool hasFragmentShader() const;
	bool hasVertexShader() const;
	template <typename resource_t> Key<ResourceProgram> add(std::string const &name, resource_t const &value);
	template <typename resource_t> Key<ResourceProgram> add(std::string &&name, resource_t &&value);
	template <typename resource_t> Program &set(Key<ResourceProgram> const &key, resource_t const &value);
	template <typename resource_t> Program &set(Key<ResourceProgram> const &key, resource_t &&value);
	template <typename resource_t> Program &has(Key<ResourceProgram> const &key);
	Program &update();

private:
	bool create(GLuint compute);
	bool create(GLuint vertex, GLuint fragment);
	bool create(GLuint vertex, GLuint fragment, GLuint geometry);
	void destroy();

private:
	size_t _layer;
	std::vector<IResourceProgram *> _resourceProgram;
	std::vector<UnitProg> _unitProg;
	GLuint _id;
	std::unique_ptr<VertexPool> _vertPool;
	std::unique_ptr<IndexPool> _idPool;
};

template <typename resource_t> 
Key<ResourceProgram> Program::add(std::string const &name, resource_t const &value)
{
	auto key = Key<ResourceProgram>::createKey(_layer);
	_resourceProgram.emplace_back(value);
	assert(_resourceProgram.size() > key.getId())
	return (key);
}

template <typename resource_t> 
Key<ResourceProgram> Program::add(std::string &&name, resource_t &&value)
{
	auto key = Key<ResourceProgram>::createKey(_layer);
	_resourceProgram.emplace_back(std::move(value));
	assert(_resourceProgram.size() > key.getId())
	return (key);
}


template <typename resource_t>
Program & Program::set(Key<ResourceProgram> const &key, resource_t const &value)
{
	_resourceProgram[key.getId()] = value;
	assert(_resourceProgram.size() > key.getId())
	return (key);
}

template <typename resource_t>
Program & Program::set(Key<ResourceProgram> const &key, resource_t &&value)
{
	_resourceProgram[key.getId()] = std::move(value);
	assert(_resourceProgram.size() > key.getId())
	return (key);
}

template <typename resource_t>
Program & Program::has(Key<ResourceProgram> const &key)
{
	if (_resourceProgram.size() > key.getId())
		return (true);
	return (false);
}
