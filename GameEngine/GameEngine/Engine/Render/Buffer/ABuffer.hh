#pragma once

#include <Render/Buffer/IBuffer.hh>
#include <string>

class ABuffer : public IBuffer
{
public:
	virtual ~ABuffer();
	ABuffer(ABuffer const &copy) = delete;
	ABuffer &operator=(ABuffer const &i) = delete;

protected:
	ABuffer();
	ABuffer(ABuffer &&move);

public:
	GLuint id() const;

public:
	virtual size_t size() const override final;

protected:
	GLuint _id;
	size_t _size;
};