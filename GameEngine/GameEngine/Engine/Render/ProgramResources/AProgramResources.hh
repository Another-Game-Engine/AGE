#pragma once

# include <Utils/OpenGL.hh>
# include <string>
# include <Render/ProgramResources/IProgramResources.hh>

class Program;

class AProgramResources : public IProgramResources
{
protected:
	AProgramResources(Program const &parent, std::string const &name, GLenum type);
	AProgramResources(Program const &parent, std::string &&name, GLenum type);
	AProgramResources(GLint id, std::string &&name, GLenum type);
	AProgramResources(AProgramResources &&move);
	AProgramResources(AProgramResources const &copy) = delete;
	AProgramResources &operator=(AProgramResources const &a) = delete;

public:
	std::string const &name() const;
	GLuint id() const;
	bool update() const;
	AProgramResources &update(bool u);
	GLenum type() const;

private:
	bool create(Program const &parent);

protected:
	std::string _name;
	GLint _id;
	GLenum _type;
	bool _update;
};