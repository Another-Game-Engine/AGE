#include <Render/ProgramResources/Types/UniformBlock.hh>
#include <iostream>

namespace AGE
{
	UniformBlock::UniformBlock(GLint id, std::string &&name, std::vector<std::unique_ptr<BlockResources>> &&blockResources, size_t sizeBuffer) :
		AProgramResources(id, std::move(name), GL_UNIFORM_BLOCK),
		AInterfaceBlock(std::move(blockResources), sizeBuffer)
	{
		for (auto &resource : _block_resources) {
			resource->assignation(this);
		}
	}

	UniformBlock::UniformBlock(GLint id, std::string &&name, std::vector<std::unique_ptr<BlockResources>> &&blockResources, AInterfaceBlock const &shared) :
		AProgramResources(id, std::move(name), GL_UNIFORM_BLOCK),
		AInterfaceBlock(std::move(blockResources), shared)
	{
		for (auto &resource : _block_resources) {
			resource->assignation(this);
		}
	}

	UniformBlock::UniformBlock(UniformBlock &&move) :
		AProgramResources(std::move(move)),
		AInterfaceBlock(std::move(move))
	{
		for (auto &resource : _block_resources) {
			resource->assignation(this);
		}
	}

	IProgramResources & UniformBlock::update()
	{
		if (_update_resource) {
			_buffer->bind();
			glBindBufferBase(_buffer->mode(), _binding_point, (GLuint)_buffer->id());
			for (auto &blockResource : _block_resources) {
				blockResource->update();
			}
			_update_resource = true;
		}
		return (*this);
	}

	size_t UniformBlock::size() const
	{
		return (sizeof(type_t));
	}

	bool UniformBlock::safe(size_t s) const
	{
		return ((size() == s) ? true : false);
	}

	void UniformBlock::print() const
	{
		std::cout << "uniform block " << _name << ";" << std::endl;
		for (auto &block_resource : _block_resources) {
			std::cout << '\t';
			block_resource->print();
		}
	}
}