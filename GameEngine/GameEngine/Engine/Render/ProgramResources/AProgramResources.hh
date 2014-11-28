#pragma once

# include <Utils/OpenGL.hh>
# include <string>
# include <Render/ProgramResources/IProgramResources.hh>

class Program;

class AProgramResources : public IProgramResource
{
protected:
	AProgramResources(Program const &parent, std::string const &name);
	AProgramResources(Program const &parent, std::string &&name);
	AProgramResources(AProgramResources &&move);
	AProgramResources(AProgramResources const &copy) = delete;
	AProgramResources &operator=(AProgramResources const &a) = delete;

public:
	std::string const &name() const;
	GLuint id() const;
	bool update() const;
	AProgramResources &update(bool u);

private:
	bool create(Program const &parent);

protected:
	std::string _name;
	GLint _id;
	bool _isUpdate;
};