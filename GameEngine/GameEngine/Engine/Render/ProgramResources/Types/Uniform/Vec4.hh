#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <glm/glm.hpp>

class Program;

class Vec4 : public AProgramResources
{
public:
	Vec4(glm::vec4 const &value, GLint id, std::string &&name);
	Vec4(Vec4 &&move);
	Vec4(Vec4 const &copy);
	Vec4 &operator=(Vec4 const &v) = delete;
	Vec4 &operator=(glm::vec4 const &value);

public:
	virtual IProgramResources &update() override final;
	virtual bool safe(size_t size) const override final;
	virtual size_t size() const override final;
	virtual void print() const override final;

public:
	typedef glm::vec4 type_t;

private:
	glm::vec4 _value;
};