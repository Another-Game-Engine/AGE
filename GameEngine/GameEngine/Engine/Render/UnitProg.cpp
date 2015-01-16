#include <Render/UnitProg.hh>
#include <assert.h>
#include <fstream>
#include <vector>

UnitProg::UnitProg(std::string const &filename, GLenum type):
_filename(filename),
_type(type)
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

UnitProg::~UnitProg()
{
	destroy();
}

bool UnitProg::compileUnitProg(char const *fileName)
{
	std::ifstream file(fileName, std::ios_base::binary);
	assert(!file.fail());
	size_t fileSize;
	file.seekg(0, file.end);
	fileSize = static_cast<GLint>(file.tellg()) + 1;
	file.seekg(0, file.beg);
	std::vector<char> content(fileSize);
	file.read(content.data(), fileSize - 1);
	content[fileSize - 1] = 0;
	GLchar const *convertionSource = content.data();
	glShaderSource(_id, 1, &convertionSource, NULL);
	glCompileShader(_id);
	GLint compileRet = 0;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &compileRet);
	if (compileRet != GL_FALSE)
		return (true);
	GLsizei msgLenght;
	glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &msgLenght);
	GLchar *errorMsg;
	errorMsg = new GLchar[msgLenght];
	glGetShaderInfoLog(_id, msgLenght, &msgLenght, errorMsg);
	std::cerr << "Compile error on " << fileName << ": " << std::endl;
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
	assert(compileUnitProg(_filename.c_str()));
}

void UnitProg::destroy() const
{
	if (_id != 0) {
		glDeleteShader(_id);
	}
}

