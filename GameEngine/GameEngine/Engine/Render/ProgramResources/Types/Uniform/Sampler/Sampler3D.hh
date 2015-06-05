#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <memory>

namespace AGE
{
	class Texture3D;

	class Sampler3D : public AProgramResources
	{
	public:
		Sampler3D(GLint id, std::string &&name);
		Sampler3D(Sampler3D &&move);
		Sampler3D(Sampler3D const &copy);
		Sampler3D &operator=(Sampler3D const &copy) = delete;
		Sampler3D &operator=(std::shared_ptr<Texture3D> const &texture);

	public:
		virtual IProgramResources & update() override final;
		virtual void print() const override final;
		virtual bool safe(size_t size) const override final;
		virtual size_t size() const override final;

	public:
		typedef Texture3D * type_t;

	private:
		std::shared_ptr<Texture3D> _texture;
	};
}