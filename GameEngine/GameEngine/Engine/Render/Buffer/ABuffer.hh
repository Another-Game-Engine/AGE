#pragma once

# include <Render/Buffer/IBuffer.hh>

class ABuffer : public IBuffer
{
public:
	ABuffer();
	virtual ~ABuffer();
	ABuffer(ABuffer &&move);
	ABuffer &operator=(ABuffer &&i);
	ABuffer(ABuffer const &copy) = delete;
	ABuffer &operator=(ABuffer const &i) = delete;

public:
	GLuint id() const;

protected:
	GLuint _id;
};