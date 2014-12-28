#include <Render/ProgramResources/AProgramResources.hh>
#include <Render/Program.hh>
#include <assert.h>
#include <array>
#include <string>

AProgramResources::AProgramResources(GLuint id, std::string &&name, GLenum type) :
_name(std::move(name)),
_id(id),
_type(type),
_update(false)
{
}

AProgramResources::AProgramResources(AProgramResources &&move) :
_name(std::move(move._name)),
_id(move._id),
_type(move._type),
_update(move._update)
{
	move._id = -1;
	_name = "";
}

AProgramResources::AProgramResources(AProgramResources const &copy) :
_name(copy._name),
_id(copy._id),
_type(copy._type),
_update(copy._update)
{

}

std::string const & AProgramResources::name() const
{
	return (_name);
}

GLuint AProgramResources::id() const
{
	return (_id);
}

GLenum AProgramResources::type() const
{
	return (_type);
}

