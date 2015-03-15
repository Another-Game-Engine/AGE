#pragma once

# include <Render/Buffer/ABuffer.hh>

class IndexBuffer : public ABuffer
{
protected:
	virtual void _bind() override final;
	virtual void _unbind(bool reset) override final;
	static IndexBuffer *_lastBinded;
public:
	virtual GLenum mode() const override final;
	virtual ABuffer const &alloc(size_t size) override final;
	virtual ABuffer const &sub(size_t offset, size_t size, void const *buffer) const override final;
};