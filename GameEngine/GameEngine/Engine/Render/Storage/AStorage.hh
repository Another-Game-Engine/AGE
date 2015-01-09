#pragma once

# include <Render/Storage/IStorage.hh>

class AStorage : public IStorages
{
protected:
	AStorage(GLint weight, GLint height, GLenum internal_format);
	AStorage(AStorage &&move);

public:
	virtual ~AStorage();

public:
	virtual GLuint id() const override final;

public:
	GLint weight() const;
	GLint height() const;
	GLenum internal_format() const;

protected:
	GLuint _id;
	GLint _weight;
	GLint _height;
	GLint _internal_format;
};