#include <Render/Program.hh>
#include <assert.h>
#include <Utils/OpenGL.hh>

Program::Program(std::vector<std::shared_ptr<UnitProg>> const &u, std::vector<Attribute> const &attibutes) :
_unitsProg(u)
{
	_graphicalMemory.init(attibutes);
	create();
}

Program::Program(Program &&move):
_resourcesProgram(std::move(move._resourcesProgram)),
_unitsProg(std::move(move._unitsProg)),
_id(std::move(move._id))
{
	move._id = 0;
}

GLuint Program::getId() const
{
	return (_id);
}

Program & Program::update()
{
	return (*this);
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

void Program::create()
{
	_id = glCreateProgram();
	for (auto &element : _unitsProg) {
		glAttachShader(_id, element->getId());
	}
	glLinkProgram(_id);
}

void Program::destroy()
{
	if (_id > 0) {
		glDeleteProgram(_id);
	}
}

Key<ProgramResource> & Program::addResource(std::unique_ptr<IProgramResources> const &value)
{
	_resourcesProgram.emplace_back(value);
	return (Key<ProgramResource>::createKey(_resourcesProgram.size() - 1));
}

std::unique_ptr<IProgramResources> const & Program::getResource(Key<ProgramResource> const &key)
{
	return (_resourcesProgram[key.getId()]);
}

bool Program::has(Key<ProgramResource> const &key)
{
	return (_resourcesProgram.size() > key.getId());
}


