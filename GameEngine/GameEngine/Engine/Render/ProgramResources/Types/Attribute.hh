#pragma once

# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/ProgramResources/Types/ProgramResourcesType.hh>
# include <Render/Buffer/VertexBuffer.hh>
# include <Render/Buffer/VertexArray.hh>
# include <Render/GeometryManagement/BlockMemory.hh>
# include <memory>

class Attribute : public AProgramResources
{
public:
	Attribute(GLint index, std::string &&name, GlType const &type);
	Attribute(Attribute &&move);
	Attribute(Attribute const &copy);
	Attribute &operator=(Attribute const &a) = delete;
	template <typename type_t> Attribute &operator+=(std::vector<type_t> const &data);

public:
	virtual IProgramResources &operator()() override final;
	virtual bool safe(size_t size) const override final;
	virtual size_t size() const override final;
	virtual void print() const override final;

public:
	typedef Attribute * type_t;

public:
	VertexBuffer const &buffer() const;
	Attribute &update();
	template <typename scalar_t> Attribute &push_back(std::vector<scalar_t> const &data);
	Attribute &pop_back();
	Attribute &bind();
	Attribute &unbind();
	BlockMemory &operator[](size_t index);

private:
	bool _update_memory;
	bool _update_alloc;
	size_t _size_alloc;
	std::vector<BlockMemory> _block_memories;
	GlType _available_type;
	VertexBuffer _buffer;
};

template <typename scalar_t>
Attribute & Attribute::push_back(std::vector<scalar_t> const &data)
{
	auto offset = _size_alloc;
	_block_memories.emplace_back(BlockMemory(*this, offset, data));
	_size_alloc += _block_memories.back().size();
	_update_alloc = false;
	_update_memory = false;
	return (*this);
}
