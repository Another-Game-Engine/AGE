#pragma once

# include <Utils/OpenGL.hh>
# include <iostream>


class UnitProg
{
public:
	UnitProg(std::string const &filename, GLenum type);
	UnitProg(UnitProg const &copy);
	UnitProg(UnitProg &&move);
	UnitProg &operator=(UnitProg const &u);
	UnitProg &operator=(UnitProg &&u);
	~UnitProg();
	GLenum getType() const;
	GLuint getId() const;

private:
	bool compileUnitProg(GLuint id, char const *file) const;
	void create();
	void destroy() const;

private:
	std::string _filename;
	GLenum _type;
	GLuint _id;
};
