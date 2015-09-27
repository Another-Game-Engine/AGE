#pragma once

# include <vector>
# include <stdint.h>
# include <memory>
# include <assert.h>
# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/ProgramResources/AInterfaceBlock.hh>
# include <Render/ProgramResources/Types/BlockResources.hh>
# include <Render/Buffer/UniformBuffer.hh>

namespace AGE
{
	class UniformBlock : public AProgramResources, public AInterfaceBlock
	{
	public:
		UniformBlock(GLint id, std::string &&name, std::vector<std::unique_ptr<BlockResources>> &&blockResources, size_t sizeBuffer);
		UniformBlock(GLint id, std::string &&name, std::vector<std::unique_ptr<BlockResources>> &&blockResources, AInterfaceBlock const &shared);
		UniformBlock(UniformBlock &&move);
		UniformBlock(UniformBlock const &copy) = delete;
		UniformBlock &operator=(UniformBlock const &u) = delete;

	public:
		virtual IProgramResources &update() override final;
		virtual void print() const override final;
		virtual size_t size() const override final;
		virtual bool safe(size_t size) const final;

	public:
		typedef UniformBlock * type_t;
	};
}