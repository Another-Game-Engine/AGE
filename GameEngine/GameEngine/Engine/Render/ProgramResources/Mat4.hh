#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/ProgramResources/ABlockResources.hh>
# include <Render/ProgramResources/UniformBlock.hh>
# include <glm/glm.hpp>

class Program;

class Mat4 : public AProgramResources, public ABlockResources
{
public:
	Mat4(glm::mat4 const &value, Program const &parent, std::string const &name);
	Mat4(glm::mat4 const &value, Program const &parent, GLint id);
	Mat4(glm::mat4 const &value, Program const &parent, std::string &&name);
	Mat4(Mat4 &&move);
	Mat4(Mat4 const &copy) = delete;
	Mat4 &operator=(Mat4 const &m) = delete;
	Mat4 &operator=(glm::mat4 const &value);

public:
	virtual IProgramResources &operator()() override final;
	virtual bool safe(size_t size) const override final;
	virtual size_t size() const override final;
	virtual void const *data() const override final;

public:
	typedef glm::mat4 type;

private:
	glm::mat4 _value;
};