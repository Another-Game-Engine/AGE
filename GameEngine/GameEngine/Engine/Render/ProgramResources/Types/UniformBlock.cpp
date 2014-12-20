#include <Render/ProgramResources/Types/UniformBlock.hh>
#include <iostream>

UniformBlock::UniformBlock(GLint id, std::string &&name, std::vector<std::shared_ptr<IBlockResources>> &&blockResources) :
AProgramResources(id, std::move(name), GL_UNIFORM_BLOCK),
_blockResources(std::move(blockResources)),
_buffer(std::make_shared<UniformBuffer>()),
_block_binding(0)
{
}

UniformBlock::UniformBlock(UniformBlock &&move) :
AProgramResources(std::move(move)),
_buffer(std::move(move._buffer)),
_block_binding(move._block_binding)
{

}

IProgramResources & UniformBlock::operator()()
{
	if (!_update) {
		for (auto &blockResource : _blockResources) {
			//_buffer->sub(blockResource->offset(), blockResource->size(), blockResource->data());
		}
		_update = true;
	}
	return (*this);
}

void UniformBlock::print() const
{
	std::cout << "uniform block " << _name << ";";
	std::cout << std::endl;
}
