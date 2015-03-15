#include <Render/Buffer/UniformBuffer.hh>

namespace AGE
{
	UniformBuffer *UniformBuffer::_lastBinded = nullptr;

	void UniformBuffer::_bind()
	{
		if (_lastBinded == this)
		{
			return;
		}
		if (_lastBinded)
		{
			_lastBinded->unbind();
		}

		_lastBinded = this;
		glBindBuffer(GL_UNIFORM_BUFFER, _id);
	}

	void UniformBuffer::_unbind()
	{
		if (_lastBinded != this)
		{
			return;
		}
		_lastBinded = nullptr;
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	GLenum UniformBuffer::mode() const
	{
		return (GL_UNIFORM_BUFFER);
	}

	ABuffer const & UniformBuffer::alloc(size_t size)
	{
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		_size = size;
		return (*this);
	}

	ABuffer const & UniformBuffer::sub(size_t offset, size_t size, void const *buffer) const
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, buffer);
		return (*this);
	}
}

