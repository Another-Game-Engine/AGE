#pragma once

# include <Utils/OpenGL.hh>
# include <glm/glm.hpp>
# include <string>

class Program;

class IResourceProgram
{
public:
	virtual ~IResourceProgram();
	virtual IResourceProgram &operator()() = 0;
};

class AResourceProgram : public IResourceProgram
{
protected:
	AResourceProgram(Program const &parent, std::string const &name);
	AResourceProgram(Program const &parent, std::string &&name);

public:
	virtual ~AResourceProgram();

public:
	Program const &getParent() const;
	std::string const &getName() const;
	GLuint getId() const;
	bool isUpdate() const;
	void setUpdate(bool isUpdate);

private:
	bool create(Program const &program);

protected:
	std::string const _name;
	GLint _id;
	bool _isUpdate;
};

class Vec4 : public AResourceProgram
{
public:
	Vec4(glm::vec4 const &value, Program const &parent, std::string const &name);
	Vec4(glm::vec4 const &value, Program const &parent, std::string &&name);
	Vec4 &operator=(glm::vec4 const &value);
public:
	typedef glm::vec4 type;

public:
	virtual IResourceProgram &operator()() override final;

private:
	glm::vec4 _value;
};

class Mat4 : public AResourceProgram
{
public:
	Mat4(glm::mat4 const &value, Program const &parent, std::string const &name);
	Mat4(glm::mat4 const &value, Program const &parent, std::string &&name);
	Mat4 &operator=(glm::mat4 const &value);

public:
	virtual IResourceProgram &operator()() override final;

public:
	typedef glm::mat4 type;

private:
	glm::mat4 _value;
};

typedef IResourceProgram ResourceProgram;