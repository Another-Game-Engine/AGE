#include <Render/UnitProg.hh>
#include <assert.h>

UnitProg::UnitProg(std::string const &filename, GLenum type):
_filename(filename),
_type(type)
{
	create();
}

UnitProg::UnitProg(UnitProg const &copy):
UnitProg(copy._filename, copy._type)
{
	create();
}

UnitProg::UnitProg(UnitProg &&move):
_filename(std::move(move._filename)),
_type(std::move(move._type)),
_id(std::move(move._id))
{
	move._type = 0;
	move._id = 0;
}

UnitProg & UnitProg::operator=(UnitProg const &u)
{
	if (&u == this)
	{
		return (*this);
	}
	destroy();
	create();
	return (*this);
}

UnitProg & UnitProg::operator=(UnitProg &&u)
{
	if (&u == this)
	{
		return (*this);
	}
	_filename = std::move(u._filename);
	_type = std::move(u._type);
	_id = std::move(u._id);
	return (*this);
}

UnitProg::~UnitProg()
{
	destroy();
}

bool UnitProg::compileUnitProg(GLuint id, char const *file)
{
	GLint compileRet = 0;
	glCompileShader(_id);
	glGetShaderiv(_id, GL_COMPILE_STATUS, &compileRet);
	if (compileRet != GL_FALSE)
		return (true);
	GLsizei msgLenght;
	glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &msgLenght);
	GLchar *errorMsg;
	errorMsg = new GLchar[msgLenght];
	glGetShaderInfoLog(_id, msgLenght, &msgLenght, errorMsg);
	std::cerr << "Compile error on " << file << ": " << std::endl;
	std::cerr << std::endl << errorMsg << std::endl << std::endl;
	delete[] errorMsg;
	return (false);
}

GLenum UnitProg::getType() const
{
	return _type;
}


GLuint UnitProg::getId() const
{
	return _id;
}

void UnitProg::create()
{
	_id = glCreateShader(_type);
	GLchar const *source = _filename.c_str();
	glShaderSource(_id, 1, &source, NULL);
	assert(compileUnitProg(_id, source));
}

void UnitProg::destroy()
{
	if (_id != 0)
		glDeleteShader(_id);
}

