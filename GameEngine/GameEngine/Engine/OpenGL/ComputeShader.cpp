#include "ComputeShader.hh"

using namespace OpenGLTools;

ComputeShader::ComputeShader()
: _csId(0)
{}

ComputeShader::~ComputeShader()
{}

bool ComputeShader::_build()
{
	return true;
}

bool ComputeShader::init(const File &file)
{
	if (!file.exists())
		return false;
	if ((_csId = addShader(file.getFullName(), GL_COMPUTE_SHADER)) == 0)
	{
		std::cerr << "Error: compute shader invalid" << std::endl;
		return (false);
	}
	_progId = glCreateProgram();
	glAttachShader(_progId, _csId);
	linkProgram();
	glDetachShader(_progId, _csId);
	glDeleteShader(_csId);
	return true;
}