#pragma once

# include <Render/ProgramResources/IProgramResources.hh>
# include <memory>

namespace AGE
{
	class TextureCubeMap;

	class Sampler3D : public IProgramResources
	{
	public:
		Sampler3D(GLint id, std::string &&name);
		Sampler3D(Sampler3D &&move);
		Sampler3D(Sampler3D const &copy);
		Sampler3D &operator=(Sampler3D const &copy) = delete;
		Sampler3D &operator=(std::shared_ptr<TextureCubeMap> const &texture);

	public:
		virtual IProgramResources & update() override final;
		virtual void print() const override final;
		virtual bool safe(size_t size) const override final;
		virtual size_t size() const override final;

	public:
		typedef TextureCubeMap * type_t;

	private:
		std::shared_ptr<TextureCubeMap> _texture;
	};
}