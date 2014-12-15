#pragma once

#include <Utils/OpenGL.hh>
#include <string>

class Program;

class IProgramResources
{
public:
	virtual IProgramResources &operator()() = 0;
	virtual std::string const &name() const = 0;
	virtual GLuint id() const = 0;
	virtual GLenum type() const = 0;
	virtual Program const &program() const = 0;
};

typedef IProgramResources ProgramResource;