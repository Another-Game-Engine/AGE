#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/ProgramResources/Types/ProgramResourcesType.hh>
# include <Render/Buffer/VertexBuffer.hh>

class Attribute : public AProgramResources
{
public:
	Attribute(GLint index, std::string &&name, GlType const &type);
	Attribute(Attribute &&move);
	Attribute(Attribute const &copy);
	Attribute &operator=(Attribute const &a) = delete;

public:
	virtual IProgramResources &operator()() override final;
	virtual bool safe(size_t size) const override final;
	virtual size_t size() const override final;
	virtual void print() const override final;

private:
	GlType _available_type;
	VertexBuffer _buffer;
};