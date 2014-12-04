#include <Render/ProgramResources/AProgramResources.hh>
#include <Render/Program.hh>
#include <assert.h>

AProgramResources::AProgramResources(Program const &parent, std::string const &name, GLenum type) :
_name(name),
_type(type),
_update(false)
{
	assert(create(parent));
}

AProgramResources::AProgramResources(Program const &parent, std::string &&name, GLenum type) :
_name(std::move(name)),
_type(type),
_update(false)
{
	assert(create(parent));
}

AProgramResources::AProgramResources(GLint id, std::string &&name, GLenum type) :
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
	move._id = 0;
}

/**
* Method:    name
* FullName:  AProgramResources::name
* Access:    public 
* Returns:   std::string const &
* Qualifier: const
* Goal:		 get the name
*/
std::string const & AProgramResources::name() const
{
	return (_name);
}


/**
* Method:    id
* FullName:  AProgramResources::id
* Access:    public 
* Returns:   GLuint
* Qualifier: const
* Goal:		 get the id
*/
GLuint AProgramResources::id() const
{
	return (_id);
}

/**
* Method:    update
* FullName:  AProgramResources::update
* Access:    public 
* Returns:   bool
* Qualifier: const
* Goal:		 get the update state
*/
bool AProgramResources::update() const
{
	return (_update);
}

/**
* Method:    update
* FullName:  AProgramResources::update
* Access:    public 
* Returns:   AProgramResources &
* Qualifier:
* Parameter: bool u
* Goal:		 set the update state
*/
AProgramResources &AProgramResources::update(bool u)
{
	_update = u;
	return (*this);
}

/**
* Method:    create
* FullName:  AProgramResources::create
* Access:    private 
* Returns:   bool
* Qualifier:
* Parameter: Program const & parent
* Goal:		 function for get the openGL index shader
* Warning	 Version 4.3 require !
*/
bool AProgramResources::create(Program const &parent)
{
	_id = glGetProgramResourceIndex(parent.getId(), _type, _name.c_str());
	return ((_id != GL_INVALID_INDEX) ? true : false);
}

/**
* Method:    type
* FullName:  AProgramResources::type
* Access:    public 
* Returns:   GLenum
* Qualifier: const
* Goal:		 get the type of the program resource
*/
GLenum AProgramResources::type() const
{
	return (_type);
}

