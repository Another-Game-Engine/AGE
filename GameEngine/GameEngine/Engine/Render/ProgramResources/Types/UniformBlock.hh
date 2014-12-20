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
	UniformBlock(GLint id, std::string &&name, std::vector<std::shared_ptr<IBlockResources>> const &blockResources);
	UniformBlock(UniformBlock &&move);
	UniformBlock(UniformBlock const &copy) = delete;
	UniformBlock &operator=(UniformBlock const &u) = delete;

public:
	virtual IProgramResources &operator()() override final;
	virtual void print() const override final;

private:
	std::vector<std::shared_ptr<IBlockResources>> _blockResources;
	std::shared_ptr<UniformBuffer> _buffer;
	GLint _block_binding;
};