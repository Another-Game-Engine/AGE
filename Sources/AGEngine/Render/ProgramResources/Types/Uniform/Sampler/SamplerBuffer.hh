#pragma once

# include <Render/ProgramResources/IProgramResources.hh>
# include <memory>

namespace AGE
{
	class TextureBuffer;

	class SamplerBuffer : public IProgramResources
	{
	public:
		SamplerBuffer(GLint id, std::string &&name);
		SamplerBuffer(SamplerBuffer &&move);
		SamplerBuffer(SamplerBuffer const &copy);
		~SamplerBuffer();
		SamplerBuffer &operator=(SamplerBuffer const &copy) = delete;
		SamplerBuffer &operator=(std::shared_ptr<TextureBuffer> const &texture);
		inline std::shared_ptr<TextureBuffer> getTexture() { return _texture; }
	public:
		virtual IProgramResources & update() override final;
		virtual void print() const override final;
		virtual bool safe(size_t size) const override final;
		virtual size_t size() const override final;

	public:
		typedef TextureBuffer * type_t;

	private:
		std::shared_ptr<TextureBuffer> _texture;
	};
}