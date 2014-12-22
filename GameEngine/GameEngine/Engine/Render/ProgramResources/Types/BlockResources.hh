#pragma once

# include <memory>
# include <Render/ProgramResources/AProgramResources.hh>
# include <vector>
# include <tuple>

class UniformBlock;

class BlockResources : public AProgramResources
{
public:
	BlockResources(GLint id, std::string &&name, GLenum type);
	BlockResources(BlockResources &&move);
	template <typename type_t> BlockResources &operator=(type_t value);
	BlockResources &operator=(BlockResources const &b) = delete;

public:
	size_t offset() const;
	size_t size_array() const;
	size_t stride() const;
	std::vector<uint8_t> const &data() const;
	BlockResources &assign(std::shared_ptr<UniformBlock> const &parent, size_t offset, size_t sizeArray, size_t stride);

public:
	virtual IProgramResources &operator()() override final;
	virtual bool safe(size_t size) const override final;
	virtual size_t size() const override final;
	virtual void print() const override final;

private:
	std::weak_ptr<UniformBlock> _parent;
	size_t _offset;
	size_t _size_array;
	size_t _stride;
	size_t _size;
	std::vector<uint8_t> _data;
};

template <typename type_t>
BlockResources & BlockResources::operator=(type_t value)
{
	// !!!!!
	_update = false;
	auto &parent = _parent->lock();
	if (parent) {
		parent->update();
	}
	return (*this);
}
