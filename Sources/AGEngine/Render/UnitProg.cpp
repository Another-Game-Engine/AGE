#include <Render/UnitProg.hh>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <string>

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

char const *UnitProg::handleRequireToken(std::string &sources)
{
	auto offset = 0;
	do
	{
		auto start = sources.find("#include(", offset);
		if (start == std::string::npos)
		{
			return sources.c_str();
		}
		auto startFile = start + 9;
		auto end = sources.find(")", startFile);
		assert(end != std::string::npos);
		auto file = sources.substr(startFile, end - startFile);
		sources.erase(start, 9 + file.size() + 1);
		file = std::string("../../Datas/Shaders/tool/" + file);
		std::fstream f(file.c_str());
		assert(!f.fail()); //Fail to open the 'file' inside Shaders/tool/
		std::ostringstream stream;
		stream << f.rdbuf();
		std::string includeSources = stream.str();
		sources.insert(start, includeSources);
	} while (true);
	return sources.c_str();
}

bool UnitProg::compileUnitProg(char const *fileName)
{
	std::ifstream file(fileName, std::ios_base::binary);
	std::cerr << fileName << std::endl;
	assert(!file.fail());
	size_t fileSize;
	file.seekg(0, file.end);
	fileSize = static_cast<GLint>(file.tellg()) + 1;
	file.seekg(0, file.beg);
	std::ostringstream stream;
	stream << file.rdbuf();
	std::string sources = stream.str();
	GLchar const *convertionSource = handleRequireToken(sources);
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

