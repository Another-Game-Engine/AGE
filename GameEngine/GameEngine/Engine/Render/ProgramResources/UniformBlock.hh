#pragma once

# include <vector>
# include <stdint.h>
# include <memory>
# include <assert.h>
# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/ProgramResources/IBlockResources.hh>
# include <Render/Buffer/UniformBuffer.hh>

class UniformBlock : public AProgramResources
{
public:
	UniformBlock(Program const &parent, std::string const &name);
	UniformBlock(Program const &parent, std::string &&name);
	UniformBlock(Program const &parent, std::string const &name, UniformBlock const &shared);
	UniformBlock(Program const &parent, std::string &&name, UniformBlock const &shaded);
	UniformBlock(UniformBlock &&move);
	UniformBlock(UniformBlock const &copy) = delete;
	UniformBlock &operator=(UniformBlock const &u) = delete;
	template <typename type_t> type_t &operator[](size_t index);

public:
	IProgramResources &operator()();

private:
	UniformBlock &introspection(Program const &program);

private:
	std::vector<std::unique_ptr<IBlockResources>> _blockResources;
	std::shared_ptr<UniformBuffer> _buffer;
	GLint _block_binding;
};

template <typename type_t>
type_t &UniformBlock::operator[](size_t index)
{
	assert(_data[index]->safe(sizeof(type_t)));
	return (static_cast<type_t>(*_data[index].get()));
}