#include <Render/Program.hh>
#include <assert.h>
#include <Utils/OpenGL.hh>

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

GLuint Program::getId() const
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

Key<ProgramResource> & Program::addResource(std::string const &name)
{
	for (size_t index = 0; index < _programResources.size(); ++index) {
		if (name == _programResources[index]->name()) {
			return (Key<ProgramResource>::createKey(index));
		}
	}
	assert(0);
	return (Key<ProgramResource>::createKey(-1));
}

IProgramResources &Program::getResource(Key<ProgramResource> const &key)
{
	return (*_programResources[key.getId()].get());
}

bool Program::has(Key<ProgramResource> const &key)
{
	return (_programResources.size() > key.getId());
}

void Program::_getProgramResources()
{
}



