#include <Render/ProgramResources/AInterfaceBlock.hh>
#include <Render/Buffer/UniformBuffer.hh>
#include <Render/ProgramResources/Types/BlockResources.hh>

namespace AGE
{
	AInterfaceBlock::AInterfaceBlock(std::vector<std::unique_ptr<BlockResources>> &&resources, size_t size) :
		_block_resources(std::move(resources)),
		_buffer(std::make_shared<UniformBuffer>()),
		_update_resource(false)
	{
		static auto binding_point = 0ull;
		_binding_point = binding_point++;
		_buffer->bind();
		_buffer->alloc(size);
	}

	AInterfaceBlock::AInterfaceBlock(std::vector<std::unique_ptr<BlockResources>> &&resources, AInterfaceBlock const &shared) :
		_block_resources(std::move(resources)),
		_buffer(shared._buffer),
		_binding_point(shared._binding_point),
		_update_resource(false)
	{
	}


	AInterfaceBlock::AInterfaceBlock(AInterfaceBlock &&move) :
		_block_resources(std::move(move._block_resources)),
		_buffer(std::move(move._buffer)),
		_binding_point(std::move(move._binding_point)),
		_update_resource(move._update_resource)
	{

	}

	IInterfaceBlock & AInterfaceBlock::require_update()
	{
		_update_resource = true;
		return (*this);
	}

	UniformBuffer const & AInterfaceBlock::buffer() const
	{
		return (*_buffer);
	}

	BlockResources * AInterfaceBlock::get_resource(std::string const &name)
	{
		for (auto &resource : _block_resources) {
			if (resource->name() == name) {
				return (resource.get());
			}
		}
		return (nullptr);
	}

	BlockResources * AInterfaceBlock::get_resource(Key<ProgramResource> const &key)
	{
		if (key.isValid())
		{
			return (nullptr);
		}
		return (_block_resources[key.getId()].get());
	}

	Key<ProgramResource> AInterfaceBlock::get_key(std::string const &name)
	{
		auto index = 0;
		for (auto &resource : _block_resources) {
			if (resource->name() == name) {
				return (Key<ProgramResource>::createKey(index));
			}
			++index;
		}
		return (Key<ProgramResource>::createKey(-1));
	}
}