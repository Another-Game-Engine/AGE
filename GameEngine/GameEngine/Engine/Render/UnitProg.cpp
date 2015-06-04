#include <Render/UnitProg.hh>
#include <assert.h>
#include <fstream>
#include <sstream>

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

char const *UnitProg::handleRequireToken(std::vector<char> const &data)
{
//	std::string data;
//	data.str();
//	auto &start = std::find(data.begin(), data.end(), '@');
//	if (start == data.end())
//	{ 
//		return data.data();
//	}
//	auto &end = std::find(++start, data.end(), '@');
//	assert(end != data.end());
//	std::string file(start, end);
//	--start;
//	std::cout << "[" << file << "]" << std::endl;
//	std::vector<char> ret(data.size() - 2 - file.size());
//	auto after = std::copy(data.begin(), --start, ret.begin());
//	std::copy(++end, data.end(), after);
	//	auto file = data.substr(start, start - end);
//	std::cout << "file ----> " << file << std::endl;
//	ret = data.substr(0, start);
//	std::cout << "cpnt 1 ----> " << ret << std::endl;
//	ret += data.substr(end, data.size() - end);
//	std::cout << "cpnt 2 ----> " << std::endl;
//	return ret.data();
	return nullptr;
}

bool UnitProg::compileUnitProg(char const *fileName)
{
	std::ifstream file(fileName, std::ios_base::binary);
	assert(!file.fail());
	size_t fileSize;
	file.seekg(0, file.end);
	fileSize = static_cast<GLint>(file.tellg()) + 1;
	file.seekg(0, file.beg);
	std::ostringstream stream;
	stream << file.rdbuf();
	std::string data = stream.str();
	GLchar const *convertionSource = data.c_str();
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

