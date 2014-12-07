#include <Render/Program.hh>
#include <assert.h>
#include <Utils/OpenGL.hh>

Program::Program(std::vector<std::shared_ptr<UnitProg>> const &u, std::vector<Attribute> const &attibutes) :
_unitsProg(u),
_resources_factory(*this)
{
	_graphicalMemory.init(attibutes);
	create();
}

Program::Program(Program &&move) :
_resourcesProgram(std::move(move._resourcesProgram)),
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

Key<ProgramResource> & Program::addResource(GLenum mode, std::string &&name)
{
	std::unique_ptr<IProgramResources> tmp = _resources_factory.build(mode, std::move(name));
	assert(tmp.get() != nullptr);
	_resourcesProgram.emplace_back(std::move(tmp));
	return (Key<ProgramResource>::createKey(int(_resourcesProgram.size() - 1)));
}

Key<ProgramResource> & Program::addResource(std::unique_ptr<IProgramResources> value)
{
	_resourcesProgram.emplace_back(std::move(value));
	return (Key<ProgramResource>::createKey(int(_resourcesProgram.size() - 1)));
}

IProgramResources &Program::getResource(Key<ProgramResource> const &key)
{
	return (*_resourcesProgram[key.getId()].get());
}

bool Program::has(Key<ProgramResource> const &key)
{
	return (_resourcesProgram.size() > key.getId());
}


