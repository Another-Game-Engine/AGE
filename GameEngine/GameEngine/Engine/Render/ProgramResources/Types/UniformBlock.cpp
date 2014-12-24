#include <Render/ProgramResources/Types/UniformBlock.hh>
#include <iostream>

UniformBlock::UniformBlock(GLint id, std::string &&name, std::vector<std::shared_ptr<BlockResources>> &&blockResources, size_t sizeBuffer) :
AProgramResources(id, std::move(name), GL_UNIFORM_BLOCK),
_block_resources(std::move(blockResources)),
_buffer(std::make_shared<UniformBuffer>())
{
	static auto binding_point = 0ull;
	_binding_point = binding_point++;
	_buffer->alloc(sizeBuffer);
}

UniformBlock::UniformBlock(GLint id, std::string &&name, std::vector<std::shared_ptr<BlockResources>> &&blockResources, UniformBlock const &shared) :
AProgramResources(id, std::move(name), GL_UNIFORM_BLOCK),
_block_resources(std::move(blockResources)),
_buffer(shared._buffer),
_binding_point(shared._binding_point)
{

}

UniformBlock::UniformBlock(UniformBlock &&move) :
AProgramResources(std::move(move)),
_buffer(std::move(move._buffer)),
_binding_point(move._binding_point)
{

}

IProgramResources & UniformBlock::operator()()
{
	if (!_update) {
		_buffer->bind();
		glBindBufferBase(_buffer->mode(), _binding_point, (GLuint)_buffer->id());
		for (auto &blockResource : _block_resources) {
			(*blockResource)();
		}
		_update = true;
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

UniformBlock & UniformBlock::update()
{
	_update = false;
	return (*this);
}

UniformBuffer const & UniformBlock::buffer() const
{
	return (*_buffer);
}

Key<ProgramResource> UniformBlock::get_key(std::string const &name)
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

BlockResources *UniformBlock::get_resource(Key<ProgramResource> const &key)
{
	if (key) {
		return (nullptr);
	}
	return (_block_resources[key.getId()].get());
}

BlockResources * UniformBlock::get_resource(std::string const &name)
{
	for (auto &resource : _block_resources) {
		if (resource->name() == name) {
			return (resource.get());
		}
	}
	return (nullptr);
}
