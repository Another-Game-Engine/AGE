#pragma once

# include <Render/ProgramResources/IProgramResources.hh>

class Program;

class AProgramResources : public IProgramResources
{
protected:
	AProgramResources(Program const &parent, std::string &&name, GLenum type);
	AProgramResources(Program const &parent, GLint id, GLenum type);
	AProgramResources(AProgramResources &&move);
	AProgramResources(AProgramResources const &copy) = delete;
	AProgramResources &operator=(AProgramResources const &a) = delete;

public:
	virtual std::string const &name() const override final;
	virtual GLuint id() const override final;
	virtual GLenum type() const override final;
	virtual Program const &program() const override final;

private:
	bool _found_id();
	bool _found_name();

protected:
	std::string _name;
	Program const &_parent;
	GLuint _id;
	GLenum _type;
	bool _update;
};