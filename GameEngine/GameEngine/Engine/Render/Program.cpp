#include <Render/Program.hh>
#include <assert.h>
#include <Utils/OpenGL.hh>
#include <Render/ProgramResources/ProgramResourcesType.hh>

Program::Program(std::vector<std::shared_ptr<UnitProg>> const &u) :
_unitsProg(u),
_graphicalMemory(nullptr),
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
_graphicalMemory(std::move(move._graphicalMemory)),
_resources_factory(*this),
_id(std::move(move._id))
{
	move._id = 0;
}

GLuint Program::id() const
{
	return (_id);
}

Program const & Program::use() const
{
	static GLint currentProgram = 0;
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
	_getProgramResources();
}

void Program::_destroy()
{
	if (_id > 0) {
		glDeleteProgram(_id);
	}
}

Key<ProgramResource> & Program::get_key(std::string const &name)
{
	for (size_t index = 0; index < _programResources.size(); ++index) {
		if (name == _programResources[index]->name()) {
			return (Key<ProgramResource>::createKey(index));
		}
	}
	assert(0); // fail !!!
	return (Key<ProgramResource>::createKey(-1));
}

IProgramResources & Program::get_resource(std::string const &name)
{
	for (size_t index = 0; index < _programResources.size(); ++index) {
		if (name == _programResources[index]->name()) {
			return (*_programResources[index].get());
		}
	}
	assert(0); // fail !!!
	return (*_programResources.back().get());
}

IProgramResources &Program::get_resource(Key<ProgramResource> const &key)
{
	return (*_programResources[key.getId()].get());
}

bool Program::has_resource(Key<ProgramResource> const &key)
{
	return (_programResources.size() > key.getId());
}

void Program::_get_resource(size_t index, GLenum resource, std::string const & buffer)
{
	GLint size = 0;
	glGetProgramResourceName(_id, resource, index, buffer.size(), &size, (GLchar *)buffer.data());
	std::string name(buffer, 0, size);
	GLenum type_prop = GL_NUM_ACTIVE_VARIABLES;
	GLint prop = 0;
	glGetProgramResourceiv(_id, resource, index, 1, &type_prop, 1, NULL, &prop);
	std::cout << "id = " << index << ", name = " << name << ", num active inside = " << prop << std::endl;
	ProgramResourcesFactory factory(*this);
	_programResources.emplace_back(factory.build(resource, _id, std::move(name));
}

void Program::_get_resources()
{
	use();
	for (auto &resource : available_resources) {
		GLint nbr_active_resources = 0;
		GLint max_name_lenght = 0;
		glGetProgramInterfaceiv(_id, resource, GL_ACTIVE_RESOURCES, &nbr_active_resources);
		glGetProgramInterfaceiv(_id, resource, GL_MAX_NAME_LENGTH, &max_name_lenght);
		std::string buffer(max_name_lenght, 0);
		for (size_t index = 0; index < nbr_active_resources; ++index) {
			_get_resource(index, resource, buffer);
		}
	}
}

Program const & Program::print_resources() const
{
	for (auto &resource : _programResources) {
		std::cout << resource->name() << std::endl;
	}
	return (*this);
}




