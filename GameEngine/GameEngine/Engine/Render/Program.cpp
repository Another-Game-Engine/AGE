#include <Render/Program.hh>
#include <assert.h>
#include <Utils/OpenGL.hh>
#include <Render/ProgramResources/Types/ProgramResourcesType.hh>
#include <Render/ProgramResources/Types/Attribute.hh>

Program::Program(std::string &&name, std::vector<std::shared_ptr<UnitProg>> const &u) :
_unitsProg(u),
_resources_factory(*this),
_name(std::move(name))
{
	_id = glCreateProgram();
	for (auto &element : _unitsProg) {
		glAttachShader(_id, element->getId());
	}
	glLinkProgram(_id);
	_get_resources();
}

Program::~Program()
{
	if (_id > 0) {
		glDeleteProgram(_id);
	}
}

Program::Program(Program &&move) :
_program_resources(std::move(move._program_resources)),
_unitsProg(std::move(move._unitsProg)),
_resources_factory(*this),
_id(move._id),
_name(std::move(move._name))
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

Key<ProgramResource> & Program::get_key(std::string const &name)
{
	for (auto index = 0; index < _program_resources.size(); ++index) {
		if (name == _program_resources[index]->name()) {
			return (Key<ProgramResource>::createKey(index));
		}
	}
	return (Key<ProgramResource>::createKey(-1));
}

bool Program::has_resource(Key<ProgramResource> const &key)
{
	return (_program_resources.size() > key.getId());
}

void Program::_get_resource(size_t index, GLenum resource, std::string const & buffer)
{
	auto size = 0;
	glGetProgramResourceName(_id, resource, GLuint(index), GLsizei(buffer.size()), (GLsizei *)&size, (GLchar *)buffer.data());
	std::string name(buffer, 0, size);
	auto element = _resources_factory.build(resource, index, std::move(name));
	if (element) {
		_program_resources.emplace_back(std::move(element));
	}
}

void Program::_get_resources()
{
	use();
	for (auto &resource : available_resources) {
		auto nbr_active_resources = 0;
		auto max_name_lenght = 0;
		glGetProgramInterfaceiv(_id, resource, GL_ACTIVE_RESOURCES, &nbr_active_resources);
		if (nbr_active_resources == 0) {
			continue;
		}
		glGetProgramInterfaceiv(_id, resource, GL_MAX_NAME_LENGTH, &max_name_lenght);
		auto buffer = std::string(max_name_lenght, 0);
		for (size_t index = 0; index < nbr_active_resources; ++index) {
			_get_resource(index, resource, buffer);
		}
	}
}

Program const & Program::print_resources() const
{
	for (auto &resource : _program_resources) {
		resource->print();
	}
	return (*this);
}


Program & Program::update()
{
	use();
	for (auto &resource : _program_resources) {
		resource->update();
	}
	return (*this);
}

size_t Program::nbr_resources() const
{
	return (_program_resources.size());
}

bool Program::coherent_attribute(std::vector<GLenum> const &p) const
{
	for (auto &resource : _program_resources) {
		auto index = resource->id();
		if (resource->type() == GL_PROGRAM_INPUT && *static_cast<Attribute *>(resource.get()) != p[index]) {
			return (false);
		}
	}
	return (true);
}

std::string const & Program::name() const
{
	return (_name);
}

bool Program::operator==(Program const &p) const
{
	if (_id == p._id) {
		return (true);
	}
	return (false);
}

bool Program::operator!=(Program const &p) const
{
	return (!(*this == p));
}

std::shared_ptr<IProgramResources> Program::get_resource_interface(std::string const &name)
{
	for (size_t index = 0; index < _program_resources.size(); ++index) {
		if (name == _program_resources[index]->name()) {
			return (_program_resources[index]);
		}
	}
	return (nullptr);
}
