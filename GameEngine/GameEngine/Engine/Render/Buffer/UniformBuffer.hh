#pragma once

# include <Render/Buffer/ABuffer.hh>

namespace AGE
{
	class UniformBuffer : public ABuffer
	{
	public:
		virtual GLenum mode() const override final;
		virtual ABuffer const &alloc(size_t size) override final;
		virtual ABuffer const &sub(size_t offset, size_t size, void const *buffer) const override final;
	protected:
		virtual void _bind() override final;
		virtual void _unbind() override final;
		static UniformBuffer *_lastBinded;
	};
}