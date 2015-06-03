#include <Render/UnitProg.hh>
#include <assert.h>
#include <fstream>
#include <vector>

#include <direct.h>

#define PATH_INCLUDE_SHADER_TOOL "../../Shader/tool/"

UnitProg::UnitProg(std::string const &filename, GLenum type):
_filename(filename),
_type(type),
_id(0)
{
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

#include <string>

void UnitProg::handleRequireToken(std::string const &data)
{
	auto start = data.find("#include(");
	if (start == std::string::npos)
	{ 
		return;
	}
	auto end = data.find(")", start);
	assert(end == std::string::npos);
	auto file = data.substr(start, end);
	std::cout << "----> " << file << std::endl;
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
	handleRequireToken(content.data());
	GLchar const *convertionSource = content.data();
	glShaderSource(_id, 1, &convertionSource, NULL);
	glCompileShader(_id);
	GLint compileRet = 0;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &compileRet);
	if (compileRet != GL_FALSE) {
		return (true);
	}
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

bool UnitProg::compile()
{
	if (_id == 0)
	{
		_id = glCreateShader(_type);
	}
	auto ret = compileUnitProg(_filename.c_str());
	return ret;
}

void UnitProg::destroy()
{
	if (_id != 0)
	{
		glDeleteShader(_id);
		_id = 0;
	}
}

