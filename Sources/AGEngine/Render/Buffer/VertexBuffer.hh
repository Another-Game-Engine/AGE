#pragma once

#include <Render/Buffer/ABuffer.hh>

class VertexBuffer : public ABuffer
{
public:
	VertexBuffer();

public:
	virtual IBuffer const &bind() const override final;
	virtual IBuffer const &unbind() const override final;
	virtual GLenum mode() const override final;
	virtual IBuffer const &alloc(size_t size) override final;
	virtual IBuffer const &sub(size_t offset, size_t size, void const *buffer) const override final;
};