#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <glm/glm.hpp>

class Program;

class Mat4 : public AProgramResources
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

private:
	glm::mat4 _value;
};