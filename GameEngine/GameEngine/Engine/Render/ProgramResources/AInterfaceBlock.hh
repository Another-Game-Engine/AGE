#pragma once

# include <memory>
# include <vector>
# include <Render/ProgramResources/IInterfaceBlock.hh>
# include <Render/ProgramResources/Types/BlockResources.hh>

class UniformBuffer;
class BlockResources;

class AInterfaceBlock : public IInterfaceBlock
{
protected:
	AInterfaceBlock(std::vector<std::unique_ptr<BlockResources>> &&resources, size_t size);
	AInterfaceBlock(std::vector<std::unique_ptr<BlockResources>> &&resources, AInterfaceBlock const &shared);
	AInterfaceBlock(AInterfaceBlock &&move);

public:
	virtual IInterfaceBlock &require_update() override final;
	virtual UniformBuffer const &buffer() const override final;
	virtual BlockResources *get_resource(std::string const &name) override final;
	virtual Key<ProgramResource> get_key(std::string const &name) override final;
	virtual BlockResources *get_resource(Key<ProgramResource> const &key) override final;

protected:
	AInterfaceBlock(){};
	virtual ~AInterfaceBlock(){};

protected:
	std::vector<std::unique_ptr<BlockResources>> _block_resources;
	std::shared_ptr<UniformBuffer> _buffer;
	size_t _binding_point;
	bool _update_resource;
};