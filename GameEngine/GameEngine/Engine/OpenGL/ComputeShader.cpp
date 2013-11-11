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
	_csId = addShader(file.getFullName(), GL_COMPUTE_SHADER);
  _progId = glCreateProgram();
  glAttachShader(_progId, _csId);
  linkProgram();
}