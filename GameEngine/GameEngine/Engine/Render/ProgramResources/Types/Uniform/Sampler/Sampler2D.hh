#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <memory>

class ITexture;

class Sampler2D : public AProgramResources
{
public:
	Sampler2D(GLint id, std::string &&name);
	Sampler2D(Sampler2D &&move);
	Sampler2D(Sampler2D const &copy);
	Sampler2D &operator=(Sampler2D const &copy) = delete;
	Sampler2D &operator=(std::shared_ptr<ITexture> const &texture);

public:
	virtual IProgramResources & update() override final;
	virtual void print() const override final;
	virtual bool safe(size_t size) const override final;
	virtual size_t size() const override final;

public:
	typedef ITexture * type_t;

private:
	std::shared_ptr<ITexture> _texture;
};