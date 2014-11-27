#include <Render/ProgramResources/AProgramResources.hh>
#include <Render/Program.hh>
#include <assert.h>

AProgramResources::AProgramResources(Program const &parent, std::string const &name) :
_name(name),
_isUpdate(false)
{
	assert(create(parent));
}

AProgramResources::AProgramResources(Program const &parent, std::string &&name) :
_name(std::move(name)),
_isUpdate(false)
{
	assert(create(parent));
}

AProgramResources::AProgramResources(AProgramResources &&move) :
_name(std::move(move._name)),
_id(move._id),
_isUpdate(move._isUpdate)
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
	return (_isUpdate);
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
	_isUpdate = u;
	return (*this);
}

