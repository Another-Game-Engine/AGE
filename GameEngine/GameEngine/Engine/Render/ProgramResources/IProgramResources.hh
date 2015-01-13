#pragma once

#include <Utils/OpenGL.hh>
#include <string>

class Program;

class IProgramResources
{
public:
	virtual IProgramResources &update() = 0;
	virtual std::string const &name() const = 0;
	virtual GLuint id() const = 0;
	virtual GLenum type() const = 0;
	virtual void print() const = 0;
	virtual bool safe(size_t size) const = 0;
	virtual size_t size() const = 0;
};

typedef IProgramResources ProgramResource;