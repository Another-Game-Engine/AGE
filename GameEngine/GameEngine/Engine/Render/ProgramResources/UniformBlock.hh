#pragma once

# include <vector>
# include <stdint.h>
# include <memory>
# include <assert.h>
# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/ProgramResources/IBlockResources.hh>

class UniformBlock : public AProgramResources
{
public:
	UniformBlock(Program const &parent, std::string const &name);
	UniformBlock(Program const &parent, std::string &&name);
	UniformBlock(UniformBlock &&move);
	UniformBlock(UniformBlock const &copy) = delete;
	UniformBlock &operator=(UniformBlock const &u) = delete;
	template <typename type_t> type_t &operator[](size_t index);

public:
	IProgramResources &operator()();

private:
	UniformBlock &introspection(Program const &program);

private:
	GLint _size_block;
	std::vector<std::unique_ptr<IBlockResources>> _blockResources;
	std::vector<uint8_t> _buffer;
};

template <typename type_t>
type_t &UniformBlock::operator[](size_t index)
{
	assert(_data[index]->safe(sizeof(type_t)));
	return (static_cast<type_t>(*_data[index].get()));
}