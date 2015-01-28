#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <memory>

namespace AGE
{
	class Texture2D;

	class Sampler2D : public AProgramResources
	{
	public:
		Sampler2D(GLint id, std::string &&name);
		Sampler2D(Sampler2D &&move);
		Sampler2D(Sampler2D const &copy);
		Sampler2D &operator=(Sampler2D const &copy) = delete;
		Sampler2D &operator=(std::shared_ptr<Texture2D> const &texture);

	public:
		virtual IProgramResources & update() override final;
		virtual void print() const override final;
		virtual bool safe(size_t size) const override final;
		virtual size_t size() const override final;

	public:
		typedef Texture2D * type_t;

	private:
		std::shared_ptr<Texture2D> _texture;
	};
}