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
	IProgramResource &operator()();

private:
	void introspection();

private:
	std::vector<std::unique_ptr<IBlockResources>> _data;
};

template <typename type_t>
type_t &UniformBlock::operator[](size_t index)
{
	assert(_data[index]->safe(sizeof(type_t)));
	return (static_cast<type_t>(*_data[index].get()));
}