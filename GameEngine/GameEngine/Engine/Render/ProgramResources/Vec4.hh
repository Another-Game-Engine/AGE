#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/ProgramResources/ABlockResources.hh>
# include <glm/glm.hpp>

class Program;

class Vec4 : public AProgramResources, public ABlockResources
{
public:
	Vec4(glm::vec4 const &value, GLint id, std::string &&name);
	Vec4(glm::vec4 const &value, Program const &parent, std::string const &name);
	Vec4(glm::vec4 const &value, Program const &parent, std::string &&name);
	Vec4(Vec4 &&move);
	Vec4 &operator=(glm::vec4 const &value);

public:
	virtual IProgramResources &operator()() override final;
	virtual bool safe(size_t size) const override final;
	virtual size_t size() const override final;

public:
	typedef glm::vec4 type;

private:
	glm::vec4 _value;
};