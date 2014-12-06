#include <Render/ProgramResources/AProgramResources.hh>
#include <Render/Program.hh>
#include <assert.h>

AProgramResources::AProgramResources(Program const &parent, std::string const &name, GLenum type) :
_name(name),
_parent(parent),
_type(type),
_update(false)
{
	assert(found_id());
}

AProgramResources::AProgramResources(Program const &parent, std::string &&name, GLenum type) :
_name(std::move(name)),
_parent(parent),
_type(type),
_update(false)
{
	assert(found_id());
}

AProgramResources::AProgramResources(Program const &parent, GLint id, GLenum type) :
_id(id),
_parent(parent),
_type(type),
_update(false)
{
	assert(found_name());
}

AProgramResources::AProgramResources(AProgramResources &&move) :
_name(std::move(move._name)),
_parent(move._parent),
_id(move._id),
_type(move._type),
_update(move._update)
{
	move._id = 0;
}

std::string const & AProgramResources::name() const
{
	return (_name);
}

GLuint AProgramResources::id() const
{
	return (_id);
}

bool AProgramResources::update() const
{
	return (_update);
}

AProgramResources &AProgramResources::update(bool u)
{
	_update = u;
	return (*this);
}

bool AProgramResources::found_id()
{
	_id = glGetProgramResourceIndex(_parent.getId(), _type, _name.c_str());
	return ((_id != GL_INVALID_INDEX) ? true : false);
}

bool AProgramResources::found_name()
{
	GLint lenght = 0;
	glGetActiveUniformsiv(_parent.getId(), 1, &_id, GL_UNIFORM_NAME_LENGTH, &lenght);
	if (lenght == 0) {
		return (false);
	}
	std::string name(lenght, 0);
	GLsizei useless;
	glGetActiveUniformName(_parent.getId(), _id, lenght, &useless, (GLchar *)name.data());
	_name = std::move(name);
	return (true);
}


GLenum AProgramResources::type() const
{
	return (_type);
}

Program const & AProgramResources::program() const
{
	return (_parent);
}


