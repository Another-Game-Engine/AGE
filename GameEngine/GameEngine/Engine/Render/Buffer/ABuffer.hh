#pragma once

# include <Render/Buffer/IBuffer.hh>

class ABuffer : public IBuffer
{
public:
	ABuffer();
	virtual ~ABuffer();
	ABuffer(ABuffer &&move);
	ABuffer(ABuffer const &copy) = delete;
	ABuffer &operator=(ABuffer const &i) = delete;

public:
	GLuint id() const;

public:
	virtual size_t size() const override final;

protected:
	GLuint _id;
	size_t _size;
};