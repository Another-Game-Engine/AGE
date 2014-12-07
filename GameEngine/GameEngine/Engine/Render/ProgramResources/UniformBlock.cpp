#include <Render/ProgramResources/UniformBlock.hh>
#include <Render/ProgramResources/BlockResourcesFactory.hh>
#include <Render/Program.hh>

UniformBlock::UniformBlock(Program const &parent, std::string const &name) :
AProgramResources(parent, name, GL_UNIFORM_BLOCK),
_buffer(std::make_shared<UniformBuffer>()),
_block_binding(0)
{
	introspection(parent);
}

UniformBlock::UniformBlock(Program const &parent, std::string &&name) :
AProgramResources(parent, std::move(name), GL_UNIFORM_BLOCK),
_buffer(std::make_shared<UniformBuffer>()),
_block_binding(0)
{
	introspection(parent);
}

UniformBlock::UniformBlock(Program const &parent, std::string const &name, UniformBlock const &shared) :
AProgramResources(parent, name, GL_UNIFORM_BLOCK),
_buffer(shared._buffer),
_block_binding(shared._block_binding)
{

}

UniformBlock::UniformBlock(Program const &parent, std::string &&name, UniformBlock const &shared) :
AProgramResources(parent, std::move(name), GL_UNIFORM_BLOCK),
_buffer(shared._buffer),
_block_binding(shared._block_binding)
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
	for (auto &blockResource : _blockResources) {
		_buffer->sub(blockResource->offset(), blockResource->size(), blockResource->data());
	}
	return (*this);
}

UniformBlock &UniformBlock::introspection(Program const &program)
{
	static GLint static_binding = 0;
	_block_binding = static_binding++;
	GLint size_block;
	glGetActiveUniformBlockiv(program.getId(), _id, GL_UNIFORM_BLOCK_DATA_SIZE, &size_block);
	_buffer->alloc(size_block);
	GLint nbrUniforms;
	glGetActiveUniformBlockiv(program.getId(), _id, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &nbrUniforms);
	_blockResources.resize(nbrUniforms);
	std::vector<GLint> indices(nbrUniforms);
	glGetActiveUniformBlockiv(program.getId(), _id, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices.data());
	for (auto &index : indices) {
		assert(index != -1);
	}
	std::vector<GLint> offsets(nbrUniforms);
	std::vector<GLint> sizes(nbrUniforms);
	std::vector<GLint> types(nbrUniforms);
	std::vector<GLint> strides(nbrUniforms);
	std::vector<GLint> nameLenght(nbrUniforms);
	glGetActiveUniformsiv(program.getId(), nbrUniforms, (const GLuint *)indices.data(), GL_UNIFORM_OFFSET, offsets.data());
	glGetActiveUniformsiv(program.getId(), nbrUniforms, (const GLuint *)indices.data(), GL_UNIFORM_SIZE, sizes.data());
	glGetActiveUniformsiv(program.getId(), nbrUniforms, (const GLuint *)indices.data(), GL_UNIFORM_TYPE, types.data());
	glGetActiveUniformsiv(program.getId(), nbrUniforms, (const GLuint *)indices.data(), GL_UNIFORM_ARRAY_STRIDE, strides.data());
	glGetActiveUniformsiv(program.getId(), nbrUniforms, (const GLuint *)indices.data(), GL_UNIFORM_NAME_LENGTH, nameLenght.data());
	BlockResourcesFactory factory(*this);
	for (GLint index = 0; index < nbrUniforms; ++index) {
		_blockResources[index] = factory.build(types[index], indices[index]);
		_blockResources[index]->offset(offsets[index]);
		_blockResources[index]->size_array(sizes[index]);
		_blockResources[index]->stride(strides[index]);
	}
	return (*this);
}
