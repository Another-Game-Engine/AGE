#pragma once

# include <Render/ProgramResources/IProgramResources.hh>

class Program;

class AProgramResources : public IProgramResources
{
protected:
	AProgramResources(GLuint id, std::string &&name, GLenum type);
	AProgramResources(AProgramResources &&move);
	AProgramResources(AProgramResources const &copy);
	AProgramResources &operator=(AProgramResources const &a) = delete;

public:
	virtual std::string const &name() const override final;
	virtual GLuint id() const override final;
	virtual GLenum type() const override final;

protected:
	std::string _name;
	GLuint _id;
	GLenum _type;
	bool _update;
};