#include <Render/Program.hh>
#include <assert.h>
#include <Utils/OpenGL.hh>
#include <Render/ProgramResources/Types/ProgramResourcesType.hh>

Program::Program(std::vector<std::shared_ptr<UnitProg>> const &u) :
_unitsProg(u),
_resources_factory(*this)
{
	_create();
}

Program::~Program()
{
	_destroy();
}

Program::Program(Program &&move) :
_programResources(std::move(move._programResources)),
_unitsProg(std::move(move._unitsProg)),
_resources_factory(*this),
_id(move._id)
{
	move._id = 0;
}

GLuint Program::id() const
{
	return (_id);
}

Program const & Program::use() const
{
	static auto currentProgram = 0;
	if (currentProgram == _id) {
		return (*this);
	}
	currentProgram = _id;
	glUseProgram(_id);
	return (*this);
}

void Program::_create()
{
	_id = glCreateProgram();
	for (auto &element : _unitsProg) {
		glAttachShader(_id, element->getId());
	}
	glLinkProgram(_id);
	_get_resources();
}

void Program::_destroy()
{
	if (_id > 0) {
		glDeleteProgram(_id);
	}
}

Key<ProgramResource> & Program::get_key(std::string const &name)
{
	for (auto index = 0; index < _programResources.size(); ++index) {
		if (name == _programResources[index]->name()) {
			return (Key<ProgramResource>::createKey(index));
		}
	}
	return (Key<ProgramResource>::createKey(-1));
}

IProgramResources * Program::get_resource(std::string const &name)
{
	for (size_t index = 0; index < _programResources.size(); ++index) {
		if (name == _programResources[index]->name()) {
			return (_programResources[index].get());
		}
	}
	return (nullptr);
}

IProgramResources * Program::get_resource(Key<ProgramResource> const &key)
{
	if (key) {
		return (nullptr);
	}
	return (_programResources[key.getId()].get());
}

bool Program::has_resource(Key<ProgramResource> const &key)
{
	return (_programResources.size() > key.getId());
}

void Program::_get_resource(size_t index, GLenum resource, std::string const & buffer)
{
	auto size = 0;
	glGetProgramResourceName(_id, resource, index, buffer.size(), &size, (GLchar *)buffer.data());
	std::string name(buffer, 0, size);
	auto element = _resources_factory.build(resource, index, std::move(name));
	if (element) {
		_programResources.emplace_back(std::move(element));
	}
}

void Program::_get_resources()
{
	use();
	for (auto &resource : available_resources) {
		auto nbr_active_resources = 0;
		auto max_name_lenght = 0;
		glGetProgramInterfaceiv(_id, resource, GL_ACTIVE_RESOURCES, &nbr_active_resources);
		glGetProgramInterfaceiv(_id, resource, GL_MAX_NAME_LENGTH, &max_name_lenght);
		auto buffer = std::string(max_name_lenght, 0);
		for (size_t index = 0; index < nbr_active_resources; ++index) {
			_get_resource(index, resource, buffer);
		}
	}
}

Program const & Program::print_resources() const
{
	for (auto &resource : _programResources) {
		resource->print();
	}
	return (*this);
}




