#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <glm/glm.hpp>

class Program;

class Mat4 : public AProgramResources
{
public:
	Mat4(glm::mat4 const &value, GLint id, std::string &&name);
	Mat4(Mat4 &&move);
	Mat4(Mat4 const &copy);
	Mat4 &operator=(Mat4 const &m) = delete;
	Mat4 &operator=(glm::mat4 const &value);

public:
	virtual IProgramResources &update() override final;
	virtual bool safe(size_t size) const override final;
	virtual size_t size() const override final;
	virtual void print() const override final;

public:
	typedef glm::mat4 type_t;

private:
	glm::mat4 _value;
};