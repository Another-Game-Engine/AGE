#include <Render/ProgramResources/AProgramResources.hh>
#include <Render/Program.hh>
#include <assert.h>
#include <array>

AProgramResources::AProgramResources(Program const &parent, std::string &&name, GLenum type) :
_name(std::move(name)),
_parent(parent),
_type(type),
_update(false)
{
	assert(_found_id());
}

AProgramResources::AProgramResources(Program const &parent, GLint id, GLenum type) :
_id(id),
_parent(parent),
_type(type),
_update(false)
{
	assert(_found_name());
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

bool AProgramResources::_found_id()
{
	_id = glGetProgramResourceIndex(_parent.getId(), _type, _name.c_str());
	return ((_id != GL_INVALID_INDEX) ? true : false);
}

bool AProgramResources::_found_name()
{
	size_t const nbr_properties = 1;
	std::array<GLenum, nbr_properties> type_properties = {GL_NAME_LENGTH};
	std::array<GLint, nbr_properties> properties = {0};
	glGetProgramResourceiv(_parent.getId(), _type, _id, nbr_properties, type_properties.data(), nbr_properties, NULL, properties.data());
	if (properties[0] == 0) {
		 (false);
	}
	std::string name(properties[0], 0);
	GLsizei useless;
	glGetProgramResourceName(_parent.getId(), _type, _id, properties[0], &useless, (GLchar *)name.data());
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


