#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/ProgramResources/IBlockResources.hh>
# include <glm/glm.hpp>

class Program;

class Mat4 : public AProgramResources, public IBlockResources
{
public:
	Mat4(glm::mat4 const &value, Program const &parent, std::string const &name);
	Mat4(glm::mat4 const &value, Program const &parent, std::string &&name);
	Mat4(Mat4 &&move);
	Mat4 &operator=(glm::mat4 const &value);

public:
	virtual IProgramResource &operator()() override final;

public:
	typedef glm::mat4 type;

public:
	virtual bool safe(size_t size) const override final;

private:
	glm::mat4 _value;
};